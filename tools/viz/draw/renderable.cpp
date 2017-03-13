#include "renderable.h"
#include <iostream>

namespace viz {
    namespace draw {
        ExplicitRenderable::ExplicitRenderable(Pipeline &&pipeline,
                                               GLenum primitive,
                                               uint32_t color,
                                               Matrix4 model,
                                               UniformMap uniforms)
        : pipeline_(std::move(pipeline))
        , primitive_(primitive)
        , color_(color)
        , model_(model)
        , uniforms_(uniforms)
        {
        }

        void ExplicitRenderable::Draw()
        {
            Context::Get().Color(color_);
            Context::Get().ModelMatrix(model_);
            auto binding = pipeline_.Bind();
            //			{
            //				auto uniform = pipeline_.program().GetUniform("center");
            //				if (uniform.IsValid()) {
            //					Vec3 f(1.f, 0.f, 0.f);
            //					glUniform3fv(uniform, 1, f.Data());
            //				}
            //			}
            uniforms_.Apply(pipeline_.program());
            binding.Draw(primitive_);
            Context::Get().ModelMatrix(Matrix4::Identity());
        }
    }
}
