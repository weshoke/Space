#ifndef SPACE_VIZ_DBG_TYPE_H
#define SPACE_VIZ_DBG_TYPE_H

#include <cxxabi.h>
#include <sstream>
#include <string>

namespace viz {
    namespace type {
        std::string Demangle(const char* name);
        std::string PrettyDemangle(const char* name);
    }
}

#endif
