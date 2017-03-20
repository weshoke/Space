#ifndef VIZ_GL_PROGRAM_H
#define VIZ_GL_PROGRAM_H

#include "attribute.h"
#include "glad/glad.h"
#include "shader.h"
#include "uniform.h"
#include <utility>

namespace viz {
    namespace gl {
        class Program {
           public:
            Program()
            : id_(glCreateProgram())
            {
            }

            Program(const Program& src) = delete;

            Program(Program&& src) { id_ = std::exchange(src.id_, 0u); }
            Program& operator=(Program&&) = delete;

            ~Program()
            {
                if (id() != 0u) {
                    glDeleteProgram(id());
                }
            }

            Program&& Attach(const Shader& shader)
            {
                glAttachShader(id(), shader.id());
                return std::move(*this);
            }

            Program&& Attach(const std::string& vertex, const std::string& fragment)
            {
                return Attach(Shader(GL_VERTEX_SHADER).Source(vertex).Compile())
                    .Attach(Shader(GL_FRAGMENT_SHADER).Source(fragment).Compile());
            }

            Program&& Attach(const std::string& vertex,
                             const std::string& geometry,
                             const std::string& fragment)
            {
                return Attach(Shader(GL_VERTEX_SHADER).Source(vertex).Compile())
                    .Attach(Shader(GL_GEOMETRY_SHADER).Source(geometry).Compile())
                    .Attach(Shader(GL_FRAGMENT_SHADER).Source(fragment).Compile());
            }

            Program&& Link()
            {
                glLinkProgram(id());
                return std::move(*this);
            }

            Program&& Use()
            {
                glUseProgram(id());
                return std::move(*this);
            }

            Uniform GetUniform(const std::string& name)
            {
                return Uniform(glGetUniformLocation(id(), name.data()));
            }

            Attribute GetAttribute(const std::string& name)
            {
                return Attribute(glGetAttribLocation(id(), name.data()));
            }

            // glGetProgram only has "iv" suffix
            GLint Get(GLenum pname)
            {
                GLint v;
                glGetProgramiv(id(), pname, &v);
                return v;
            }

            bool Linked() { return !!Get(GL_LINK_STATUS); }
            std::string Log()
            {
                constexpr auto max_log_length_stack = GLint{1024};
                auto size = GLsizei{0};
                auto log_length = Get(GL_INFO_LOG_LENGTH);
                if (log_length > max_log_length_stack) {
                    auto heap_log = std::vector<GLchar>(log_length, '\0');
                    glGetProgramInfoLog(id(), heap_log.size(), &size, heap_log.data());
                    return std::string(heap_log.data());
                }

                auto log = std::array<GLchar, max_log_length_stack>();
                glGetProgramInfoLog(id(), log.size(), &size, log.data());
                return std::string(log.data());
            }

            GLuint id() { return id_; }
            operator GLuint() { return id(); }
           private:
            GLuint id_;
        };
    }
}

#endif
