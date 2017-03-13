#ifndef SPACE_VIZ_DRAW_UNIFORM_MAP_H
#define SPACE_VIZ_DRAW_UNIFORM_MAP_H

#include "mpark/variant.hpp"
#include "primitives.h"
#include <string>
#include <unordered_map>

namespace viz {
    namespace draw {
        class UniformMap {
           public:
            using Value = mpark::variant<float, Vec2, Vec3>;

            UniformMap() {}
            UniformMap(const std::unordered_map<std::string, Value> &uniforms)
            : uniforms_(uniforms)
            {
            }

            UniformMap &Add(const std::string &name, Value value)
            {
                uniforms_.emplace(name, value);
                return *this;
            }

            void Apply(gl::Program &program)
            {
                for (const auto &p : uniforms_) {
                    auto uniform = program.GetUniform(std::get<0>(p));
                    if (uniform.IsValid()) {
                        Apply(uniform, std::get<1>(p));
                    }
                }
            }

           private:
            void Apply(gl::Uniform uniform, const Value &value)
            {
                switch (value.index()) {
                    case 0:
                        glUniform1fv(uniform, 1, &mpark::get<0>(value));
                        break;
                    case 1:
                        glUniform2fv(uniform, 1, mpark::get<1>(value).Data());
                        break;
                    case 2:
                        glUniform3fv(uniform, 1, mpark::get<2>(value).Data());
                        break;
                }
            }

            std::unordered_map<std::string, Value> uniforms_;
        };
    }
}

#endif
