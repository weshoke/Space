#include "renderable.h"
#include <iostream>

namespace viz {
    namespace draw {
        ExplicitRenderable::ExplicitRenderable(Pipeline &&pipeline,
                                               GLenum primitive,
                                               uint32_t color,
                                               Matrix4 model,
                                               UniformMap uniforms,
                                               std::vector<Context::TextureRef> textures)
        : pipeline_(std::move(pipeline))
        , primitive_(primitive)
        , color_(color)
        , model_(model)
        , uniforms_(uniforms)
        , textures_(textures)
        {
        }

        void ExplicitRenderable::Draw()
        {
            Context::Get().Color(color_);
            Context::Get().ModelMatrix(model_);
            auto binding = pipeline_.Bind();
            uniforms_.Apply(pipeline_.program());
            auto texture_bindings = std::array<gl::Texture::Binding, 16>();
            for (auto i = 0u; i < textures_.size(); ++i) {
                texture_bindings[i] = textures_[i]->Bind(GL_TEXTURE_2D, i);
            }
            binding.Draw(primitive_);
            Context::Get().ModelMatrix(Matrix4::Identity());
        }
    }
}
