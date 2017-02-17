#ifndef SPACE_VIZ_DRAW_CONTEXT_H
#define SPACE_VIZ_DRAW_CONTEXT_H

#include "color.h"
#include "draw.h"
#include "gl/program.h"
#include "primitives.h"
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
            void ModelViewMatrix(const Matrix4 &model_view) { model_view_ = model_view; }
            void ProjectionMatrix(const Matrix4 &projection) { projection_ = projection; }
            void SetProgramUniforms(gl::Program &program)
            {
                {
                    auto uniform = program.GetUniform("MVP");
                    if (uniform.IsValid()) {
                        auto mvp = projection_ * model_view_;
                        glUniformMatrix4fv(uniform, 1, GL_FALSE, mvp.values().data());
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
            , model_view_(Matrix4::Identity())
            , projection_(Matrix4::Identity())
            , color_(Colors::black)
            {
            }

            std::unordered_map<std::string, uint32_t> attributes_;
            std::unordered_map<std::string, gl::Program::Ref> programs_;

            Matrix4 model_view_;
            Matrix4 projection_;
            uint32_t color_;
        };
    }
}

#endif
