#ifndef SPACE_VIZ_DRAW_ANALYZED_PROGRAM_H
#define SPACE_VIZ_DRAW_ANALYZED_PROGRAM_H

#include "gl/program.h"
#include <memory>
#include <unordered_map>

namespace viz {
    namespace draw {
        class AnalyzedProgram {
           public:
            AnalyzedProgram(gl::Program&& program)
            : program_(std::move(program))
            {
                Setup();
            }

            AnalyzedProgram(const AnalyzedProgram&) = delete;

            AnalyzedProgram(AnalyzedProgram&& src)
            : program_(std::move(src.program_))
            , uniform_names_(std::move(src.uniform_names_))
            {
            }

            AnalyzedProgram& operator=(AnalyzedProgram&& src)
            {
                program_ = std::move(src.program_);
                uniform_names_ = std::move(src.uniform_names_);
                return *this;
            }

            gl::Uniform GetUniform(const std::string& name)
            {
                auto it = uniform_names_.find(name);
                return it == uniform_names_.end() ? gl::Uniform() : std::get<1>(*it);
            }

            gl::Program& program() { return program_; }
            operator GLuint() { return GLuint(program()); }
           private:
            void Setup() { StoreNonBlockUniformNames(); }
            void StoreNonBlockUniformNames()
            {
                auto count = program().Get(GL_ACTIVE_UNIFORMS);
                uniform_names_.reserve(count);
                for (auto i = 0; i < count; ++i) {
                    constexpr auto max_name_size = GLsizei{512};
                    auto name = std::array<char, max_name_size>();
                    auto name_size = GLsizei{0};
                    glGetActiveUniformName(program(), i, max_name_size, &name_size, name.data());
                    auto block_index = glGetUniformBlockIndex(program(), name.data());
                    if (block_index == GL_INVALID_INDEX) {
                        auto uniform_name = std::string(name.data());
                        uniform_names_.emplace(uniform_name, program().GetUniform(uniform_name));
                    }
                }
            }

            gl::Program program_;
            std::unordered_map<std::string, gl::Uniform> uniform_names_;
        };
    }
}

#endif
