#ifndef VIZ_GL_UNIFORM_H
#define VIZ_GL_UNIFORM_H

#include "glad/glad.h"

namespace viz {
    namespace gl {
        class Uniform {
           public:
            Uniform()
            : id_(-1)
            {
            }

            Uniform(GLint id)
            : id_(id)
            {
            }

            GLint id() const { return id_; }
            operator GLint() { return id(); }
            bool IsValid() const { return id_ >= 0; }
            operator bool() { return IsValid(); }
           private:
            GLint id_;
        };
    }
}

#endif
