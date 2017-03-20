#ifndef SPACE_VIZ_DRAW_CONTEXT_H
#define SPACE_VIZ_DRAW_CONTEXT_H

#include "analyzed_program.h"
#include "camera.h"
#include "color.h"
#include "draw.h"
#include "gl/program.h"
#include "primitives.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace viz {
    namespace draw {
        class Context {
           public:
            using ProgramRef = std::shared_ptr<AnalyzedProgram>;

            static Context &Get()
            {
                static auto ctx = Context();
                return ctx;
            }

            uint32_t AttributeLocation(const std::string &name) { return attributes_[name]; }
            void ModelMatrix(const Matrix4 &model) { model_ = model; }
            void ViewMatrix(const Matrix4 &view) { view_ = view; }
            void ProjectionMatrix(const Matrix4 &projection) { projection_ = projection; }
            void ScreenSize(const Vec2 &screen_size) { screen_size_ = screen_size; }
            void ApplyCamera(const Camera &camera)
            {
                ViewMatrix(camera.ViewMatrix());
                ProjectionMatrix(camera.ProjectionMatrix());
                lens_angle_ = camera.lens_angle();
                z_near_ = camera.near();
                z_far_ = camera.far();
            }
            void SetProgramUniforms(gl::Program &program)
            {
                {
                    auto uniform = program.GetUniform("MVP");
                    if (uniform.IsValid()) {
                        auto mvp = projection_ * view_ * model_;
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
                {
                    auto uniform = program.GetUniform("view_matrix");
                    if (uniform.IsValid()) {
                        glUniformMatrix4fv(uniform, 1, GL_FALSE, view_.values().data());
                    }
                }
                {
                    auto uniform = program.GetUniform("model_matrix");
                    if (uniform.IsValid()) {
                        glUniformMatrix4fv(uniform, 1, GL_FALSE, model_.values().data());
                    }
                }
                {
                    auto uniform = program.GetUniform("aspect");
                    if (uniform.IsValid()) {
                        auto aspect = screen_size_[0] / screen_size_[1];
                        glUniform1fv(uniform, 1, &aspect);
                    }
                }
                {
                    auto uniform = program.GetUniform("fov_y_scale");
                    if (uniform.IsValid()) {
                        auto fov_y_scale = float(std::tan(lens_angle_ * M_PI / 180.f * 0.5f));
                        glUniform1fv(uniform, 1, &fov_y_scale);
                    }
                }
                {
                    auto uniform = program.GetUniform("z_near");
                    if (uniform.IsValid()) {
                        glUniform1fv(uniform, 1, &z_near_);
                    }
                }
                {
                    auto uniform = program.GetUniform("z_far");
                    if (uniform.IsValid()) {
                        glUniform1fv(uniform, 1, &z_far_);
                    }
                }
            }

            void Color(uint32_t color) { color_ = color; }
            void RegisterProgram(const std::string &name,
                                 const std::string &vertex,
                                 const std::string &fragment)
            {
                programs_.emplace(name,
                                  std::make_shared<AnalyzedProgram>(
                                      gl::Program().Attach(vertex, fragment).Link()));
            }

            void RegisterProgram(const std::string &name,
                                 const std::string &vertex,
                                 const std::string &geometry,
                                 const std::string &fragment)
            {
                programs_.emplace(name,
                                  std::make_shared<AnalyzedProgram>(
                                      gl::Program().Attach(vertex, geometry, fragment).Link()));
            }

            ProgramRef Program(const std::string &name) { return programs_.at(name); }
           private:
            Context()
            : attributes_{{"pos", 0}}
            , model_(Matrix4::Identity())
            , view_(Matrix4::Identity())
            , projection_(Matrix4::Identity())
            , color_(Colors::black)
            {
            }

            std::unordered_map<std::string, uint32_t> attributes_;
            std::unordered_map<std::string, ProgramRef> programs_;

            Vec2 screen_size_;
            float lens_angle_;
            float z_near_;
            float z_far_;
            Matrix4 model_;
            Matrix4 view_;
            Matrix4 projection_;
            uint32_t color_;
        };
    }
}

#endif
