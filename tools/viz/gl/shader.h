#ifndef VIZ_GL_SHADER_H
#define VIZ_GL_SHADER_H

#include "glad/glad.h"
#include "primitives.h"
#include <string>
#include <utility>

namespace viz {
    namespace gl {
        class Shader {
           public:
            Shader(GLenum type)
            : id_(Create(type))
            {
            }

            Shader(const Shader& src) = delete;
            Shader(Shader&& src) { id_ = std::exchange(src.id_, 0u); }
            Shader& operator=(Shader&& src)
            {
                Delete();
                id_ = std::exchange(src.id_, 0u);
                return *this;
            }
            ~Shader() { Delete(); }
            Shader&& Source(const std::string& source)
            {
                const auto* data = source.data();
                glShaderSource(id(), 1, &data, NULL);
                return std::move(*this);
            }

            Shader&& Compile()
            {
                glCompileShader(id());
                return std::move(*this);
            }

            GLint Get(GLenum pname)
            {
                GLint v;
                glGetShaderiv(id(), pname, &v);
                return v;
            }

            bool Compiled() { return !!Get(GL_COMPILE_STATUS); }
            std::string Log() { return GlslLog(id(), Get(GL_INFO_LOG_LENGTH), glGetShaderInfoLog); }
            GLuint id() const { return id_; }
            operator GLuint() { return id(); }
           private:
            static GLuint Create(GLenum type) { return glCreateShader(type); }
            void Delete()
            {
                if (id() != 0u) {
                    glDeleteShader(id());
                }
            }
            GLuint id_;
        };
    }
}

#endif
