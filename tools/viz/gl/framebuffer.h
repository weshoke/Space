#ifndef VIZ_GL_FRAMEBUFFER_H
#define VIZ_GL_FRAMEBUFFER_H

#include "gl/renderbuffer.h"
#include "gl/texture.h"
#include "glad/glad.h"
#include <vector>

namespace viz {
    namespace gl {
        class Framebuffer {
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
                ~Binding() { Unbind(); }
                Binding &&Renderbuffer(GLenum attachment,
                                       const gl::Renderbuffer &renderbuffer,
                                       GLint level = 0)
                {
                    glFramebufferRenderbuffer(target(), attachment, GL_RENDERBUFFER, renderbuffer);
                    return std::move(*this);
                }

                Binding &&Texture(GLenum attachment, const gl::Texture &texture, GLint level = 0)
                {
                    glFramebufferTexture(target(), attachment, texture, level);
                    return std::move(*this);
                }

                Binding &&DrawBuffers(const std::vector<GLenum> &buffers)
                {
                    glDrawBuffers(buffers.size(), buffers.data());
                    return std::move(*this);
                }

                GLenum target() const { return target_; }
                GLuint id() const { return id_; }
                operator GLint() { return id(); }
               private:
                static void Bind(GLenum target, GLuint id) { glBindFramebuffer(target, id); }
                void Unbind()
                {
                    if (id() != 0u) {
                        Bind(target(), 0u);
                    }
                }
                GLenum target_;
                GLuint id_;
            };

            Framebuffer()
            : id_(0u)
            {
                Gen();
            }

            Framebuffer(const Framebuffer &src) = delete;
            Framebuffer(Framebuffer &&src) { id_ = std::exchange(src.id_, 0u); }
            Framebuffer &operator=(Framebuffer &&src)
            {
                Delete();
                id_ = std::exchange(src.id_, 0u);
                return *this;
            }
            ~Framebuffer() { Delete(); }
            Binding Bind(GLenum target) { return Binding(target, id()); }
            GLuint id() const { return id_; }
            operator GLuint() { return id(); }
           private:
            void Gen() { glGenFramebuffers(1, &id_); }
            void Delete()
            {
                if (id_ != 0u) {
                    glDeleteFramebuffers(1, &id_);
                }
            }
            GLuint id_;
        };
    }
}

#endif
