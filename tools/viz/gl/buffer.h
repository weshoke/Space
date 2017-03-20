#ifndef VIZ_GL_BUFFER_H
#define VIZ_GL_BUFFER_H

#include "glad/glad.h"
#include <utility>
#include <vector>

namespace viz {
    namespace gl {
        class Buffer {
           public:
            class Binding {
               public:
                Binding(GLenum target, GLuint id)
                : target_(target)
                , id_(id)
                {
                    Bind(this->target(), this->id());
                }

                Binding(const Binding &src) = delete;

                Binding(Binding &&src)
                : target_(src.target_)
                {
                    id_ = std::exchange(src.id_, 0u);
                }

                Binding &operator=(Binding &&) = delete;

                // TODO: somehow this binding needs to be attached to the VAO and "unbound" with
                // that object
                ~Binding()
                {
                    if (id() != 0u) {
                        // Unbind(target());
                    }
                }

                template <class Data>
                Binding &&Data(const Data &data, GLenum usage = GL_STATIC_DRAW)
                {
                    using T = typename Data::value_type;
                    glBufferData(target(), sizeof(T) * data.size(), data.data(), usage);
                    return std::move(*this);
                }

                GLenum target() const { return target_; }
                GLuint id() const { return id_; }
                operator GLint() { return id(); }
               private:
                static void Bind(GLenum target, GLuint id) { glBindBuffer(target, id); }
                static void Unbind(GLenum target) { glBindBuffer(target, 0u); }
                GLenum target_;
                GLuint id_;
            };

            Buffer()
            : id_(0u)
            {
                glGenBuffers(1, &id_);
            }
            Buffer(const Buffer &) = delete;

            Buffer(Buffer &&src) { id_ = std::exchange(src.id_, 0u); }
            Buffer &operator=(Buffer &&) = delete;
            ~Buffer()
            {
                if (id_ != 0u) {
                    glDeleteBuffers(1, &id_);
                }
            }

            Binding Bind(GLenum target) { return Binding(target, id()); }
            GLuint id() { return id_; }
            operator GLint() { return id(); }
           private:
            GLuint id_;
        };
    }
}

#endif
