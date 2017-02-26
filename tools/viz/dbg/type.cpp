#include "type.h"
#include <cxxabi.h>
#include <sstream>

namespace viz {
    namespace dbg {
        std::string Demangle(const char* name)
        {
            int status = -4;  // some arbitrary value to eliminate the compiler warning
            // enable c++11 by passing the flag -std=c++11 to g++
            std::unique_ptr<char, void (*)(void*)> res{
                abi::__cxa_demangle(name, NULL, NULL, &status), std::free};
            return (status == 0) ? res.get() : name;
        }

        std::string PrettyDemangle(const char* name)
        {
            auto ss = std::stringstream();
            auto lvl = 0;
            const auto indent = [&]() { ss << std::string(lvl * 2, ' '); };
            for (auto c : demangle(name)) {
                switch (c) {
                    case '<':
                        ss << c << "\n";
                        ++lvl;
                        indent();
                        break;

                    case '>':
                        ss << "\n";
                        --lvl;
                        indent();
                        ss << c;
                        break;

                    case ',':
                        ss << "\n";
                        indent();
                        ss << c;
                        break;

                    default:
                        ss << c;
                        break;
                }
            }
            return ss.str();
        }
    }
}
