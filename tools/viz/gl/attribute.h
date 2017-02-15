#ifndef VIZ_GL_ATTRIBUTE_H
#define VIZ_GL_ATTRIBUTE_H

#include "glad/glad.h"
#include <vector>

namespace viz {
    namespace gl {
        // TODO: AttributeGroup to manage offset automatically
        class Attribute {
           public:
            class Layout {
               public:
                Layout(GLint size, GLenum type, GLsizei stride = 0, void* offset = nullptr)
                : size_(size)
                , type_(type)
                , stride_(stride)
                , offset_(offset)
                {
                }

                GLuint size() const { return size_; }
                GLenum type() const { return type_; }
                GLsizei stride() const { return stride_; }
                void* offset() const { return offset_; }
               private:
                GLuint size_;
                GLenum type_;
                GLsizei stride_;
                void* offset_;
            };

            Attribute(GLint id)
            : id_(id)
            {
            }

            Attribute& Enable()
            {
                glEnableVertexAttribArray(id());
                return *this;
            }

            Attribute& Pointer(const Layout& layout)
            {
                glVertexAttribPointer(GLuint(id()),
                                      layout.size(),
                                      layout.type(),
                                      GL_FALSE,
                                      layout.stride(),
                                      layout.offset());
                return *this;
            }

            GLint id() const { return id_; }
            operator GLint() { return id(); }
           private:
            GLint id_;
        };
    }
}

#endif
