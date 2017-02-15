#ifndef VIZ_GL_UNIFORM_H
#define VIZ_GL_UNIFORM_H

#include "glad/glad.h"

namespace viz {
    namespace gl {
        class Uniform {
           public:
            Uniform(GLint id)
            : id_(id)
            {
            }

            GLint id() { return id_; }
            operator GLint() { return id(); }
            bool IsValid() { return id_ >= 0; }
           private:
            GLint id_;
        };
    }
}

#endif
