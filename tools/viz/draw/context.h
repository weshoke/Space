#ifndef SPACE_VIZ_DRAW_CONTEXT_H
#define SPACE_VIZ_DRAW_CONTEXT_H

#include "color.h"
#include "draw.h"
#include "gl/program.h"
#include "linmath.h"
#include <string>
#include <unordered_map>

namespace viz {
    namespace draw {
        class Context {
           public:
            static Context &Get()
            {
                static auto ctx = Context();
                return ctx;
            }

            uint32_t AttributeLocation(const std::string &name) { return attributes_[name]; }
            void ModelViewMatrix(const mat4x4 &model_view)
            {
                for (auto j = 0; j < 4; ++j) {
                    for (auto i = 0; i < 4; ++i) {
                        model_view_[j][i] = model_view[j][i];
                    }
                }
            }

            void ProjectionMatrix(const mat4x4 &projection)
            {
                for (auto j = 0; j < 4; ++j) {
                    for (auto i = 0; i < 4; ++i) {
                        projection_[j][i] = projection[j][i];
                    }
                }
            }

            void SetProgramUniforms(gl::Program &program)
            {
                {
                    auto uniform = program.GetUniform("MVP");
                    if (uniform.IsValid()) {
                        mat4x4 mvp;
                        mat4x4_mul(mvp, projection_, model_view_);
                        glUniformMatrix4fv(uniform, 1, GL_FALSE, (const GLfloat *)mvp);
                    }
                }
                {
                    auto uniform = program.GetUniform("color");
                    if (uniform.IsValid()) {
                        glUniform4fv(uniform, 1, ColorComponents(color_).data());
                    }
                }
            }

            void Color(uint32_t color) { color_ = color; }
            void RegisterProgram(const std::string &name,
                                 const std::string &vertex,
                                 const std::string &fragment)
            {
                auto program = std::make_shared<gl::Program>();
                program->Attach(vertex, fragment).Link();
                programs_.emplace(name, program);
            }

            gl::Program::Ref Program(const std::string &name) { return programs_.at(name); }
           private:
            Context()
            : attributes_{{"pos", 0}}
            , color_(Colors::black)
            {
                mat4x4_identity(model_view_);
                mat4x4_identity(projection_);
            }

            std::unordered_map<std::string, uint32_t> attributes_;
            std::unordered_map<std::string, gl::Program::Ref> programs_;

            mat4x4 model_view_;
            mat4x4 projection_;
            uint32_t color_;
        };
    }
}

#endif
