#ifndef SPACE_VIZ_DRAW_RENDERABLE_H
#define SPACE_VIZ_DRAW_RENDERABLE_H

#include "draw.h"
#include "pipeline.h"
#include <memory>

namespace viz {
    namespace draw {
        class Renderable {
           public:
            using Ref = std::shared_ptr<Renderable>;

            virtual void Draw() = 0;
        };

        class ExplicitRenderable final : public Renderable {
           public:
            ExplicitRenderable(Pipeline &&pipeline, GLenum primitive, uint32_t color);
            virtual ~ExplicitRenderable() {}
            void Draw();

           private:
            Pipeline pipeline_;
            uint64_t primitive_;
            uint32_t color_;
        };
    }
}

#endif
