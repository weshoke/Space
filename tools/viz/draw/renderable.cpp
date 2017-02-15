#include "renderable.h"
#include <iostream>

namespace viz {
    namespace draw {
        ExplicitRenderable::ExplicitRenderable(Pipeline &&pipeline,
                                               GLenum primitive,
                                               uint32_t color)
        : pipeline_(std::move(pipeline))
        , primitive_(primitive)
        , color_(color)
        {
        }

        void ExplicitRenderable::Draw()
        {
            Context::Get().Color(color_);
            pipeline_.Bind().Draw(primitive_);
        }
    }
}
