#ifndef SPACE_VIZ_DRAW_ANALYZED_PROGRAM_H
#define SPACE_VIZ_DRAW_ANALYZED_PROGRAM_H

#include "gl/program.h"
#include <memory>
#include <unordered_set>

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

            gl::Program& program() { return program_; }
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
                        uniform_names_.emplace(name.data());
                    }
                }
            }

            gl::Program program_;
            std::unordered_set<std::string> uniform_names_;
        };
    }
}

#endif
