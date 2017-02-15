#include "app/app.h"
#include "draw/color.h"
#include "draw/draw.h"
#include "draw/pipeline.h"
#include "draw/renderable.h"
#include "draw/renderable_factory.h"
#include "linmath.h"

static const char* vertex_shader_text =
    "#version 410\n"
    "uniform mat4 MVP;\n"
    "in vec2 pos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(pos, 0.0, 1.0);\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 410\n"
    "uniform vec4 color;\n"
    "out vec4 pixel;\n"
    "void main()\n"
    "{\n"
    "    pixel = color;\n"
    "}\n";

class C1 {
   public:
    using App = app::App<C1>;

    C1() {}
    void Init(App* app)
    {
        std::cout << glGetString(GL_VERSION) << "\n";
        viz::draw::Context::Get().RegisterProgram(
            "color", vertex_shader_text, fragment_shader_text);

        auto verts = std::vector<viz::draw::Vec2>{viz::draw::Vec2(-0.6f, -0.4f),
                                                  viz::draw::Vec2(0.6f, -0.4f),
                                                  viz::draw::Vec2(0.0f, 0.6f)};
        auto pipeline = viz::draw::Pipeline::Create("color", verts);
        renderables_.emplace_back(std::make_shared<viz::draw::ExplicitRenderable>(
            std::move(pipeline), GL_TRIANGLES, viz::draw::Colors::grass));
        renderables_.emplace_back(viz::draw::Create(
            viz::draw::LineSegment(viz::draw::Vec3(-1.f, 0.f, 0.f), viz::draw::Vec3(1.f, 0.f, 0.f)),
            viz::draw::Colors::black));
    }

    void Key(App* app, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        // std::cout << "KEY\n";
    }

    void Mouse(App* app, int32_t button, int32_t action, int32_t mods)
    {
        // std::cout << "Mouse\n";
    }

    void Cursor(App* app, double xpos, double ypos)
    {
        // std::cout << "Mouse: " << xpos << "," << ypos << "\n";
    }

    void Draw(App* app)
    {
        auto window_size = app->WindowSize();
        glViewport(0, 0, window_size[0], window_size[1]);
        glClearColor(0.93f, 0.93f, 0.93f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto ratio = float(window_size[0]) / float(window_size[1]);

        mat4x4 m, p;
        mat4x4_identity(m);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        viz::draw::Context::Get().ModelViewMatrix(m);
        viz::draw::Context::Get().ProjectionMatrix(p);
        for (auto& renderable : renderables_) {
            renderable->Draw();
        }
    }

    std::vector<viz::draw::Renderable::Ref> renderables_;
};

int main(void) { return C1::App::Create(C1(), "C1", 640, 480)->Run(); }
