#ifndef VIZ_GL_SHADER_H
#define VIZ_GL_SHADER_H

#include "glad/glad.h"
#include <string>
#include <utility>

namespace viz {
    namespace gl {
        class Shader {
           public:
            Shader(GLenum type)
            : id_(glCreateShader(type))
            {
            }

            Shader(const Shader& src) = delete;
            Shader(Shader&& src) { id_ = std::exchange(src.id_, 0u); }
            ~Shader()
            {
                if (id() != 0u) {
                    glDeleteShader(id());
                }
            }

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

            GLint Geti(GLenum pname)
            {
                GLint v;
                glGetShaderiv(id(), pname, &v);
                return v;
            }

            bool Compiled() { return !!Geti(GL_COMPILE_STATUS); }
            std::string Log()
            {
                auto log = std::array<GLchar, 1024>();
                auto size = GLsizei{0};
                glGetShaderInfoLog(id(), log.size(), &size, log.data());
                return std::string(log.data());
            }

            GLuint id() const { return id_; }
            operator GLuint() { return id(); }
           private:
            GLuint id_;
        };
    }
}

#endif
