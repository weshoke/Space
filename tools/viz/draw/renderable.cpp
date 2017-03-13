#include "renderable.h"
#include <iostream>

namespace viz {
    namespace draw {
        ExplicitRenderable::ExplicitRenderable(Pipeline &&pipeline,
                                               GLenum primitive,
                                               uint32_t color,
                                               Matrix4 model)
        : pipeline_(std::move(pipeline))
        , primitive_(primitive)
        , color_(color)
        , model_(model)
        {
        }

        void ExplicitRenderable::Draw()
        {
            Context::Get().Color(color_);
            Context::Get().ModelMatrix(model_);
            pipeline_.Bind().Draw(primitive_);
            Context::Get().ModelMatrix(Matrix4::Identity());
        }
    }
}
