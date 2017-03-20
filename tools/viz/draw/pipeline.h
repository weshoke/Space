#ifndef VIZ_DRAW_PIPELINE_H
#define VIZ_DRAW_PIPELINE_H

#include "context.h"
#include "gl/program.h"
#include "mesh.h"
#include <memory>
#include <unordered_set>

// TODO: cache all uniforms in unordered_set
namespace viz {
    namespace draw {
        class Pipeline {
           public:
            class Binding {
               public:
                Binding(Pipeline& pipeline)
                : pipeline_(pipeline)
                , mesh_binding_(pipeline.mesh_.Bind())
                {
                }

                Binding(const Binding& src) = delete;

                Binding(Binding&& src)
                : pipeline_(src.pipeline_)
                , mesh_binding_(std::move(src.mesh_binding_))
                {
                }

                Binding& operator=(Binding&&) = delete;

                Binding&& Draw(GLenum primitive)
                {
                    mesh_binding_.Draw(primitive);
                    return std::move(*this);
                }

               private:
                Pipeline& pipeline_;
                Mesh::Binding mesh_binding_;
            };

            Pipeline(Context::ProgramRef program, Mesh&& mesh)
            : program_(program)
            , mesh_(std::move(mesh))
            {
                Setup();
            }

            Pipeline(const Pipeline& src) = delete;

            Pipeline(Pipeline&& src)
            : program_(std::move(src.program_))
            , mesh_(std::move(src.mesh_))
            {
            }

            Pipeline& operator=(Pipeline&& src)
            {
                program_ = std::move(src.program_);
                mesh_ = std::move(src.mesh_);
                return *this;
            }

            template <class Data>
            static Pipeline Create(const std::string& program_name, const Data& vertex)
            {
                return Pipeline(Context::Get().Program(program_name),
                                Mesh().Bind([&vertex](auto&& binding) { binding.Vertex(vertex); }));
            }

            template <class VertexData, class IndexData>
            static Pipeline Create(const std::string& program_name,
                                   const VertexData& vertex,
                                   const IndexData& index)
            {
                return Pipeline(Context::Get().Program(program_name),
                                Mesh().Bind([&vertex, &index](auto&& binding) {
                                    binding.Vertex(vertex).Index(index);
                                }));
            }

            static Pipeline Create(const std::string& program_name, Mesh&& mesh)
            {
                return Pipeline(Context::Get().Program(program_name), std::move(mesh));
            }

            Pipeline&& Program(const std::string& vertex, const std::string& fragment)
            {
                program().Attach(vertex, fragment).Link().Use();
                return std::move(*this);
            }

            // Pipeline&& BindAttributes()
            // {
            //     // TODO: iterator over all context attributes
            //     glBindAttribLocation(program(), Context::Get().AttributeLocation("pos"), "pos");
            // }

            Binding Bind()
            {
                program().Use();
                Context::Get().SetProgramUniforms(program());
                return Binding(*this);
            }

            gl::Program& program() { return *program_; }
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

            // Using ProgramRef since programs are shared across many Pipelines and stored globally
            // in a key-value map in Context
            Context::ProgramRef program_;
            Mesh mesh_;
            std::unordered_set<std::string> uniform_names_;
        };
    }
}

#endif
