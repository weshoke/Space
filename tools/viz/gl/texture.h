#ifndef VIZ_GL_TEXTURE_H
#define VIZ_GL_TEXTURE_H

#include "glad/glad.h"
#include <vector>

namespace viz {
    namespace gl {
        class Texture {
           public:
            class Binding {
               public:
                Binding()
                : target_(GL_TEXTURE_2D)
                , id_(0)
                {
                }

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

                Binding &operator=(Binding &&src)
                {
                    target_ = src.target_;
                    id_ = std::exchange(src.id_, 0u);
                    return *this;
                }

                ~Binding() { Unbind(); }
                template <class Image>
                Binding &&ImageData(GLenum internal_format,
                                    GLenum format,
                                    GLsizei width,
                                    GLsizei height,
                                    const Image &image)
                {
                    using T = typename Image::value_type;
                    glTexImage2D(target(),
                                 0,
                                 internal_format,
                                 width,
                                 height,
                                 0,
                                 format,
                                 Type<T>(),
                                 image.data());
                    glGenerateMipmap(target());
                    return std::move(*this);
                }

                Binding &&Image(GLenum internal_format,
                                GLenum format,
                                GLenum type,
                                GLsizei width,
                                GLsizei height)
                {
                    glTexImage2D(
                        target(), 0, internal_format, width, height, 0, format, type, nullptr);
                    glGenerateMipmap(target());
                    return std::move(*this);
                }

                Binding &&Parameter(GLenum pname, GLint param)
                {
                    glTexParameteri(target(), pname, param);
                    return std::move(*this);
                }

                GLenum target() const { return target_; }
                GLuint id() const { return id_; }
                operator GLint() { return id(); }
               private:
                static void Bind(GLenum target, GLuint id) { glBindTexture(target, id); }
                void Unbind()
                {
                    if (id() != 0u) {
                        Bind(target(), 0u);
                    }
                }
                GLenum target_;
                GLuint id_;
            };

            Texture(GLenum target = GL_TEXTURE_2D)
            : target_(target)
            , id_(0u)
            {
                Gen();
            }

            Texture(const Texture &src) = delete;
            Texture(Texture &&src) { id_ = std::exchange(src.id_, 0u); }
            Texture &operator=(Texture &&src)
            {
                Delete();
                id_ = std::exchange(src.id_, 0u);
                return *this;
            }
            ~Texture() { Delete(); }
            static void Active(uint32_t idx) { glActiveTexture(GL_TEXTURE0 + idx); }
            Binding Bind()
            {
                Active(0);
                return Binding(target(), id());
            }
            Binding Bind(uint32_t idx)
            {
                Active(idx);
                return Binding(target(), id());
            }

            GLenum target() const { return target_; }
            GLuint id() const { return id_; }
            operator GLuint() const & { return id(); }
           private:
            void Gen() { glGenTextures(1, &id_); }
            void Delete()
            {
                if (id_ != 0u) {
                    glDeleteTextures(1, &id_);
                }
            }
            GLenum target_;
            GLuint id_;
        };
    }
}

#endif
