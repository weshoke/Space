#ifndef VIZ_MESH_DRAW_H
#define VIZ_MESH_DRAW_H

#include "context.h"
#include "gl/buffer.h"
#include "gl/primitives.h"
#include "gl/vertex_array.h"
#include "glad/glad.h"
#include "primitives.h"
#include <utility>
#include <vector>

namespace viz {
    namespace draw {
        class Mesh {
           public:
            class Binding {
               public:
                Binding(Mesh& mesh, gl::VertexArray::Binding&& binding)
                : mesh_(mesh)
                , binding_(std::move(binding))
                {
                }

                Binding(const Binding& src) = delete;

                Binding(Binding&& src)
                : mesh_(src.mesh_)
                , binding_(std::move(src.binding_))
                {
                }

                template <class Data>
                Binding&& Vertex(const Data& data)
                {
                    using T = typename Data::value_type;
                    mesh_.count_ = data.size();
                    mesh_.vbos_.emplace_back(gl::Buffer());
                    auto binding = mesh_.vbos_.back().Bind(GL_ARRAY_BUFFER).Data(data);
                    gl::Attribute(Context::Get().AttributeLocation("pos"))
                        .Enable()
                        .Pointer(gl::Attribute::Layout(draw::Size<T>(), GL_FLOAT));
                    return std::move(*this);
                }

                template <class Data>
                Binding&& Index(const Data& data)
                {
                    using T = typename Data::value_type;
                    mesh_.count_ = data.size();
                    mesh_.type_ = gl::Type<T>();
                    mesh_.vbos_.emplace_back(gl::Buffer());
                    auto binding = mesh_.vbos_.back().Bind(GL_ELEMENT_ARRAY_BUFFER).Data(data);
                    return std::move(*this);
                }

                Binding&& Draw(GLenum primitive)
                {
                    // TODO: wrap in GL primitives
                    if (mesh_.HasIndexBuffer()) {
                        glDrawElements(primitive, mesh_.count_, mesh_.type_, nullptr);
                    }
                    else {
                        glDrawArrays(primitive, 0, mesh_.count_);
                    }
                    return std::move(*this);
                }

               private:
                Mesh& mesh_;
                gl::VertexArray::Binding binding_;
            };

            Mesh() {}
            Mesh(const Mesh& src) = delete;

            Mesh(Mesh&& src)
            : count_(src.count_)
            , type_(src.type_)
            , vao_(std::move(src.vao_))
            , vbos_(std::move(src.vbos_))
            {
            }

            Mesh& operator=(Mesh&&) = delete;

            Binding Bind() { return Binding(*this, vao_.Bind()); }
           private:
            bool HasIndexBuffer() { return vbos_.size() >= 2; }
            GLint count_;
            GLenum type_;
            gl::VertexArray vao_;
            std::vector<gl::Buffer> vbos_;
        };
    }
}

#endif
