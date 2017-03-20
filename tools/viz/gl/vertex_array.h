#ifndef VIZ_GL_VERTEX_ARRAY_H
#define VIZ_GL_VERTEX_ARRAY_H

#include "glad/glad.h"
#include <memory>
#include <utility>
#include <vector>

namespace viz {
    namespace gl {
        class VertexArray {
           public:
            using Ref = std::shared_ptr<VertexArray>;

            class Binding {
               public:
                Binding(GLuint id)
                : id_(id)
                {
                    Bind(this->id());
                }

                Binding(const Binding& src) = delete;
                Binding(Binding&& src) { id_ = std::exchange(src.id_, 0u); }
                ~Binding()
                {
                    if (id() != 0u) {
                        Bind(0u);
                    }
                }

                static void Bind(GLuint id) { glBindVertexArray(id); }
                GLuint id() const { return id_; }
                operator GLuint() { return id(); }
               private:
                GLuint id_;
            };

            VertexArray()
            : id_(0u)
            {
                glGenVertexArrays(1, &id_);
            }

            VertexArray(const VertexArray& src) = delete;

            VertexArray(VertexArray&& src) { id_ = std::exchange(src.id_, 0u); }
            VertexArray& operator=(VertexArray&&) = delete;
            ~VertexArray()
            {
                if (id_ != 0u) {
                    glDeleteVertexArrays(1, &id_);
                }
            }

            Binding Bind() { return Binding(id()); }
            GLuint id() const { return id_; }
            operator GLuint() { return id(); }
           private:
            GLuint id_;
        };
    }
}

#endif
