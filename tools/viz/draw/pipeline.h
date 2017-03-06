#ifndef VIZ_MESH_PIPELINE_H
#define VIZ_MESH_PIPELINE_H

#include "context.h"
#include "gl/program.h"
#include "mesh.h"

// TODO: us monadic constructs for binding/using
// composition of monadic constructs for pipeline to mirror the internal
// composition of resources

namespace viz {
    namespace draw {
        class Pipeline {
           public:
            class Binding {
               public:
                Binding(Pipeline& pipeline, Mesh::Binding&& mesh_binding)
                : pipeline_(pipeline)
                , mesh_binding_(std::move(mesh_binding))
                {
                }

                Binding(const Binding& src) = delete;

                Binding(Binding&& src)
                : pipeline_(src.pipeline_)
                , mesh_binding_(std::move(src.mesh_binding_))
                {
                }

                // TODO: figure out how to do hierarchical binding.  Monads? .push? .pop?
                template <class F>
                Binding&& Mesh(F&& f)
                {
                    f(mesh_binding_);
                    return std::move(*this);
                }

                Binding&& Link()
                {
                    // TODO: iterator over context attributes and see if they exist in the program
                    // If so, set their location
                    glBindAttribLocation(
                        *(pipeline_.program_), Context::Get().AttributeLocation("pos"), "pos");
                    return std::move(*this);
                }

                Binding&& Draw(GLenum primitive)
                {
                    mesh_binding_.Draw(primitive);
                    return std::move(*this);
                }

               private:
                Pipeline& pipeline_;
                Mesh::Binding mesh_binding_;
            };

            Pipeline()
            : Pipeline(std::make_shared<gl::Program>())
            {
            }

            Pipeline(gl::Program::Ref program)
            : program_(program)
            {
            }

            Pipeline(const Pipeline& src) = delete;

            Pipeline(Pipeline&& src)
            : program_(std::move(src.program_))
            , mesh_(std::move(src.mesh_))
            {
            }

            template <class Data>
            static Pipeline Create(const std::string& program, const Data& vertex)
            {
                auto pipeline = Pipeline(Context::Get().Program(program));
                pipeline.Bind().Mesh([&vertex](auto&& binding) { binding.Vertex(vertex); });
                return pipeline;
            }

            template <class VertexData, class IndexData>
            static Pipeline Create(const std::string& program,
                                   const VertexData& vertex,
                                   const IndexData& index)
            {
                auto pipeline = Pipeline(Context::Get().Program(program));
                pipeline.Bind().Mesh(
                    [&vertex, &index](auto&& binding) { binding.Vertex(vertex).Index(index); });
                return pipeline;
            }

            Pipeline& Program(const std::string& vertex, const std::string& fragment)
            {
                program_->Attach(vertex, fragment).Link().Use();
                return *this;
            }

            Binding Bind()
            {
                program_->Use();
                Context::Get().SetProgramUniforms(*program_);
                return Binding(*this, mesh_.Bind());
            }

            gl::Program& program() { return *program_; }
           private:
            gl::Program::Ref program_;
            Mesh mesh_;
        };
    }
}

#endif
