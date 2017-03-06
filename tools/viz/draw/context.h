#ifndef SPACE_VIZ_DRAW_CONTEXT_H
#define SPACE_VIZ_DRAW_CONTEXT_H

#include "camera.h"
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
            void ScreenSize(const Vec2 &screen_size) { screen_size_ = screen_size; }
            void ApplyCamera(const Camera &camera)
            {
                auto m = camera.ModelViewMatrix();
                ModelViewMatrix(m);
                ProjectionMatrix(camera.ProjectionMatrix());
            }
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
                {
                    auto uniform = program.GetUniform("screen_size");
                    if (uniform.IsValid()) {
                        glUniform2fv(uniform, 1, screen_size_.values.data());
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

            void RegisterProgram(const std::string &name,
                                 const std::string &vertex,
                                 const std::string &geometry,
                                 const std::string &fragment)
            {
                auto program = std::make_shared<gl::Program>();
                program->Attach(vertex, geometry, fragment).Link();
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

            Vec2 screen_size_;
            Matrix4 model_view_;
            Matrix4 projection_;
            uint32_t color_;
        };
    }
}

#endif
