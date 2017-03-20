#ifndef VIZ_GL_PRIMITIVES_H
#define VIZ_GL_PRIMITIVES_H

#include "glad/glad.h"
#include <string>

namespace viz {
    namespace gl {
        template <class T>
        GLenum Type();

        bool Error(const std::string &msg);
    }
}

#endif
