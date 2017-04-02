#ifndef VIZ_DRAW_MESH_H
#define VIZ_DRAW_MESH_H

#include "Optional/optional.hpp"
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
                Binding(Mesh& mesh)
                : mesh_(mesh)
                , binding_(mesh.vao_.Bind())
                {
                }

                Binding(const Binding& src) = delete;

                Binding(Binding&& src)
                : mesh_(src.mesh_)
                , binding_(std::move(src.binding_))
                {
                }

                Binding& operator=(Binding&&) = delete;

                template <class Data>
                Binding&& Vertex(const Data& data)
                {
                    if (!mesh_.index_buffer_) {
                        mesh_.count_ = data.size();
                    }
                    return Attribute(data, "pos");
                }

                template <class Data>
                Binding&& TexCoord(const Data& data)
                {
                    return Attribute(data, "tex_coord");
                }

                template <class Data>
                Binding&& Index(const Data& data)
                {
                    using T = typename Data::value_type;
                    mesh_.count_ = data.size();
                    mesh_.type_ = gl::Type<T>();
                    mesh_.index_buffer_ =
                        gl::Buffer().Bind(GL_ELEMENT_ARRAY_BUFFER,
                                          [&data](auto&& binding) { binding.Data(data).Detach(); });
                    return std::move(*this);
                }

                Binding&& Draw(GLenum primitive)
                {
                    // TODO: wrap in GL primitives
                    if (mesh_.index_buffer_) {
                        glDrawElements(primitive, mesh_.count_, mesh_.type_, nullptr);
                    }
                    else {
                        glDrawArrays(primitive, 0, mesh_.count_);
                    }
                    return std::move(*this);
                }

               private:
                template <class Data>
                Binding&& Attribute(const Data& data, const std::string& name)
                {
                    using T = typename Data::value_type;
                    mesh_.vbos_.emplace_back(
                        gl::Buffer().Bind(GL_ARRAY_BUFFER, [&data, &name](auto&& binding) {
                            binding.Data(data);
                            gl::Attribute(Context::Get().AttributeLocation(name))
                                .Enable()
                                .Pointer(gl::Attribute::Layout(draw::Size<T>(), GL_FLOAT));
                            binding.Detach();
                        }));
                    return std::move(*this);
                }

                Mesh& mesh_;
                gl::VertexArray::Binding binding_;
            };

            Mesh() {}
            Mesh(const Mesh& src) = delete;

            Mesh(Mesh&& src)
            : count_(src.count_)
            , type_(src.type_)
            , vao_(std::move(src.vao_))
            , index_buffer_(std::move(src.index_buffer_))
            , vbos_(std::move(src.vbos_))
            {
            }

            Mesh& operator=(Mesh&& src)
            {
                count_ = src.count_;
                type_ = src.type_;
                vao_ = std::move(src.vao_);
                index_buffer_ = std::move(src.index_buffer_);
                vbos_ = std::move(src.vbos_);
                return *this;
            }

            Binding Bind() { return Binding(*this); }
            template <class F>
            Mesh&& Bind(F&& f)
            {
                f(Bind());
                return std::move(*this);
            }

           private:
            GLint count_;
            GLenum type_;
            gl::VertexArray vao_;
            std::experimental::optional<gl::Buffer> index_buffer_;
            std::vector<gl::Buffer> vbos_;
        };
    }
}

#endif
