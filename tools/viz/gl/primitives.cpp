#include "primitives.h"
#include <iostream>

namespace viz {
    namespace gl {
        template <>
        GLenum Type<uint8_t>()
        {
            return GL_UNSIGNED_BYTE;
        }
        template <>
        GLenum Type<uint16_t>()
        {
            return GL_UNSIGNED_SHORT;
        }
        template <>
        GLenum Type<uint32_t>()
        {
            return GL_UNSIGNED_INT;
        }

        bool Error(const std::string &msg)
        {
            auto err = glGetError();
            switch (err) {
                case GL_INVALID_ENUM:
                    std::cout << msg << " err: GL_INVALID_ENUM\n";
                    break;

                case GL_INVALID_VALUE:
                    std::cout << msg << " err: GL_INVALID_VALUE\n";
                    break;

                case GL_INVALID_OPERATION:
                    std::cout << msg << " err: GL_INVALID_OPERATION\n";
                    break;

                default:
                    std::cout << msg << " err: " << err << "\n";
                    break;
            }
            return err != GL_NO_ERROR;
        };
    }
}
