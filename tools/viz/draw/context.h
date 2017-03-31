#ifndef SPACE_VIZ_DRAW_CONTEXT_H
#define SPACE_VIZ_DRAW_CONTEXT_H

#include "analyzed_program.h"
#include "camera.h"
#include "color.h"
#include "draw.h"
#include "gl/program.h"
#include "primitives.h"
#include "shader_preprocessor.h"
#include <iostream>
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

            void SetProgramUniforms(AnalyzedProgram &program)
            {
                for (const auto &p : uniforms_) {
                    const auto &name = std::get<0>(p);
                    if (auto uniform = program.GetUniform(name)) {
                        auto fn = std::get<1>(p);
                        (this->*fn)(uniform);
                    }
                }
            }

            void Color(uint32_t color) { color_ = color; }
            template <class F1, class F2>
            void RegisterProgram(const std::string &name, F1 &&load_file, F2 &&file_exists)
            {
                const auto compile_shader = [](gl::Shader shader,
                                               const std::string &type,
                                               const std::string &name,
                                               auto &&load_file) {
                    auto source = Preprocess(name, load_file);
                    shader.Source(source.lines()).Compile();
                    if (!shader.Compiled()) {
                        std::cout << "Error compiling " << type << " shader " << name << "\n";
                        std::cout << source.Error(shader.Log());
                    }
                    return shader;
                };

                auto vertex_shader =
                    compile_shader(gl::Shader(GL_VERTEX_SHADER), "vertex", name + ".vs", load_file);
                auto fragment_shader = compile_shader(
                    gl::Shader(GL_FRAGMENT_SHADER), "fragment", name + ".fs", load_file);

                auto program = gl::Program()
                                   .Attach(std::move(vertex_shader))
                                   .Attach(std::move(fragment_shader));
                if (file_exists(name + ".gs")) {
                    auto geometry_shader = compile_shader(
                        gl::Shader(GL_GEOMETRY_SHADER), "geometry", name + ".gs", load_file);
                    program.Attach(std::move(geometry_shader));
                }
                program.Link();
                VerifyProgram(program, name);
                programs_.emplace(name, std::make_shared<AnalyzedProgram>(std::move(program)));
            }

            ProgramRef Program(const std::string &name) { return programs_.at(name); }
           private:
            Context()
            : attributes_{{"pos", 0}}
            , uniforms_{{"MVP", &Context::ModelViewProjectionMatrixUniform},
                        {"view_matrix", &Context::ViewMatrixUniform},
                        {"model_matrix", &Context::ModelMatrixUniform},
                        {"color", &Context::ColorUniform},
                        {"screen_size", &Context::ScreenSizeUniform},
                        {"aspect", &Context::AspectUniform},
                        {"fov_y_scale", &Context::FovYScaleUniform},
                        {"z_near", &Context::ZNearUniform},
                        {"z_far", &Context::ZFarUniform}}
            , model_(Matrix4::Identity())
            , view_(Matrix4::Identity())
            , projection_(Matrix4::Identity())
            , color_(Colors::black)
            {
            }

            void VerifyProgram(gl::Program &program, const std::string &name)
            {
                if (!program.Linked()) {
                    std::cout << "Error linking program " << name << "\n";
                    std::cout << program.Log() << "\n";
                }
            }

            void ModelViewProjectionMatrixUniform(gl::Uniform uniform)
            {
                auto mvp = projection_ * view_ * model_;
                glUniformMatrix4fv(uniform, 1, GL_FALSE, mvp.values().data());
            }

            void ViewMatrixUniform(gl::Uniform uniform)
            {
                glUniformMatrix4fv(uniform, 1, GL_FALSE, view_.values().data());
            }

            void ModelMatrixUniform(gl::Uniform uniform)
            {
                glUniformMatrix4fv(uniform, 1, GL_FALSE, model_.values().data());
            }

            void ColorUniform(gl::Uniform uniform)
            {
                glUniform4fv(uniform, 1, ColorComponents(color_).data());
            }

            void ScreenSizeUniform(gl::Uniform uniform)
            {
                glUniform2fv(uniform, 1, screen_size_.values.data());
            }

            void AspectUniform(gl::Uniform uniform)
            {
                auto aspect = screen_size_[0] / screen_size_[1];
                glUniform1fv(uniform, 1, &aspect);
            }

            void FovYScaleUniform(gl::Uniform uniform)
            {
                auto fov_y_scale = float(std::tan(lens_angle_ * M_PI / 180.f * 0.5f));
                glUniform1fv(uniform, 1, &fov_y_scale);
            }

            void ZNearUniform(gl::Uniform uniform) { glUniform1fv(uniform, 1, &z_near_); }
            void ZFarUniform(gl::Uniform uniform) { glUniform1fv(uniform, 1, &z_far_); }
            typedef void (Context::*UniformFn)(gl::Uniform uniform);

            std::unordered_map<std::string, uint32_t> attributes_;
            std::unordered_map<std::string, ProgramRef> programs_;
            std::unordered_map<std::string, UniformFn> uniforms_;

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
