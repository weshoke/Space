#include "type.h"

namespace viz {
    namespace type {
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
            auto d_name = Demangle(name);
            auto it = d_name.begin();
            auto ite = d_name.end();
            auto lvl = 0;
            const auto indent = [&]() {
                for (auto i = 0; i < lvl; ++i) {
                    ss << "  ";
                }
            };
            while (it != ite) {
                switch (*it) {
                    case '<':
                        ss << *it;
                        ss << "\n";
                        ++lvl;
                        indent();
                        break;

                    case '>':
                        ss << "\n";
                        --lvl;
                        indent();
                        ss << *it;
                        break;

                    case ',':
                        ss << "\n";
                        indent();
                        ss << *it;
                        break;

                    default:
                        ss << *it;
                        break;
                }
                ++it;
            }
            return ss.str();
        }
    }
}
