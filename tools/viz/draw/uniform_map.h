#ifndef SPACE_VIZ_DRAW_UNIFORM_MAP_H
#define SPACE_VIZ_DRAW_UNIFORM_MAP_H

#include "mpark/variant.hpp"
#include "primitives.h"
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

namespace viz {
    namespace draw {
        class UniformMap {
           public:
            // Large union type that wastes space but provides a nice interface
            // Under the hood, values will be packed into a tight buffer.
            using Value = mpark::variant<float, Vec2, Vec3, Vec4, Matrix3, Matrix4>;

           private:
            enum class ValueType { Float = 0, Vec2, Vec3, Vec4, Matrix3, Matrix4 };

            struct Entry {
                ValueType value_type;
                uint32_t index;
            };

            ValueType ValueTypeFromValue(const Value& value)
            {
                return static_cast<ValueType>(value.index());
            }

           public:
            UniformMap() {}
            UniformMap& Add(const std::string& name, Value value)
            {
                uniforms_.emplace(name, Entry{ValueTypeFromValue(value), AppendValue(value)});
                return *this;
            }

            void Apply(AnalyzedProgram& program)
            {
                for (const auto& p : uniforms_) {
                    auto uniform = program.GetUniform(std::get<0>(p));
                    if (uniform.IsValid()) {
                        auto entry = std::get<1>(p);
                        Apply(uniform, entry.value_type, EntryPointer(entry));
                    }
                }
            }

           private:
            const float* EntryPointer(Entry entry) const { return values_.data() + entry.index; }
            static uint32_t ValueTypeCount(ValueType value_type)
            {
                switch (value_type) {
                    case ValueType::Float:
                        return 1u;
                    case ValueType::Vec2:
                        return 2u;
                    case ValueType::Vec3:
                        return 3u;
                    case ValueType::Vec4:
                        return 4u;
                    case ValueType::Matrix3:
                        return 9u;
                    case ValueType::Matrix4:
                        return 16u;
                }
                return 0u;
            }

            static const float* ValueData(const Value& value)
            {
                switch (value.index()) {
                    case 0:
                        return &mpark::get<0>(value);
                    case 1:
                        return mpark::get<1>(value).Data();
                    case 2:
                        return mpark::get<2>(value).Data();
                    case 3:
                        return mpark::get<3>(value).Data();
                    case 4:
                        return mpark::get<4>(value).Data();
                    case 5:
                        return mpark::get<5>(value).Data();
                }
                return nullptr;
            }

            uint32_t AppendValue(const Value& value)
            {
                auto offset = size_t(values_.size());
                auto count = ValueTypeCount(ValueTypeFromValue(value));
                values_.resize(offset + count);
                std::memcpy(values_.data() + offset, ValueData(value), count * sizeof(float));
                return offset;
            }

            static void Apply(gl::Uniform uniform, ValueType value_type, const float* value)
            {
                switch (value_type) {
                    case ValueType::Float:
                        glUniform1fv(uniform, 1, value);
                        break;
                    case ValueType::Vec2:
                        glUniform2fv(uniform, 1, value);
                        break;
                    case ValueType::Vec3:
                        glUniform3fv(uniform, 1, value);
                        break;
                    case ValueType::Vec4:
                        glUniform4fv(uniform, 1, value);
                        break;
                    case ValueType::Matrix3:
                        glUniformMatrix3fv(uniform, 1, GL_FALSE, value);
                        break;
                    case ValueType::Matrix4:
                        glUniformMatrix4fv(uniform, 1, GL_FALSE, value);
                        break;
                }
            }

            std::vector<float> values_;
            std::unordered_map<std::string, Entry> uniforms_;
        };
    }
}

#endif
