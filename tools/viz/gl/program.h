#ifndef VIZ_GL_PROGRAM_H
#define VIZ_GL_PROGRAM_H

#include "attribute.h"
#include "glad/glad.h"
#include "primitives.h"
#include "shader.h"
#include "uniform.h"
#include <utility>

namespace viz {
    namespace gl {
        class Program {
           public:
            Program()
            : id_(Create())
            {
            }

            Program(const Program& src) = delete;

            Program(Program&& src) { id_ = std::exchange(src.id_, 0u); }
            Program& operator=(Program&& src)
            {
                Delete();
                id_ = std::exchange(src.id_, 0u);
                return *this;
            }

            ~Program() { Delete(); }
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

            // glGetProgram only has "iv" suffix, so function doesn't need to be templated and
            // specialized
            GLint Get(GLenum pname)
            {
                GLint v;
                glGetProgramiv(id(), pname, &v);
                return v;
            }

            bool Linked() { return !!Get(GL_LINK_STATUS); }
            std::string Log()
            {
                return GlslLog(id(), Get(GL_INFO_LOG_LENGTH), glGetProgramInfoLog);
            }

            GLuint id() { return id_; }
            operator GLuint() { return id(); }
           private:
            static GLuint Create() { return glCreateProgram(); }
            void Delete()
            {
                if (id() != 0u) {
                    glDeleteProgram(id());
                }
            }

            GLuint id_;
        };
    }
}

#endif
