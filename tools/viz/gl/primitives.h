#ifndef VIZ_GL_PRIMITIVES_H
#define VIZ_GL_PRIMITIVES_H

#include "glad/glad.h"
#include <array>
#include <string>
#include <vector>

namespace viz {
    namespace gl {
        template <class T>
        GLenum Type();

        template <class T>
        GLenum InternalFormat();

        bool Error(const std::string& msg);

        template <class F>
        std::string GlslLog(GLuint id, GLint log_length, F&& info_log)
        {
            constexpr auto max_log_length_stack = GLint{1024};
            auto size = GLsizei{0};
            if (log_length > max_log_length_stack) {
                auto heap_log = std::vector<GLchar>(log_length, '\0');
                info_log(id, heap_log.size(), &size, heap_log.data());
                return std::string(heap_log.data());
            }

            auto log = std::array<GLchar, max_log_length_stack>();
            info_log(id, log.size(), &size, log.data());
            return std::string(log.data());
        }
    }
}

#endif
