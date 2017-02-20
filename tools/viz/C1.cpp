#include "app/app.h"
#include "draw/camera.h"
#include "draw/color.h"
#include "draw/draw.h"
#include "draw/pipeline.h"
#include "draw/renderable.h"
#include "draw/renderable_factory.h"
#include "draw/trackball.h"
#include "geom/primitives.h"
#include <cxxabi.h>
#include <sstream>

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

std::string demangle(const char* name)
{
    int status = -4;  // some arbitrary value to eliminate the compiler warning
    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void (*)(void*)> res{abi::__cxa_demangle(name, NULL, NULL, &status),
                                               std::free};
    return (status == 0) ? res.get() : name;
}

std::string pretty_demangle(const char* name)
{
    auto ss = std::stringstream();
    auto d_name = demangle(name);
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

class C1 {
   public:
    using App = app::App<C1>;

    C1()
    : camera(viz::draw::Camera::Default())
    , trackball(viz::draw::Trackball(1.f, 1.f))
    , mouse_down(false)
    , mouse_drag(false)
    {
    }

    void Init(App* app)
    {
        using Vec2 = viz::draw::Vec2;
        using Vec3 = viz::draw::Vec3;
        //		std::cout << space::product::PrintOp(Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f, 0.f)) <<
        //"\n";
        //		std::cout << "\n\n";
        //		std::cout << space::product::PrintGp(Vec3(0.f, 0.f, 0.f) ^ Vec3(0.f, 0.f, 0.f),
        // Vec3(0.f, 0.f, 0.f)) << "\n";
        std::cout << glGetString(GL_VERSION) << "\n";
        viz::draw::Context::Get().RegisterProgram(
            "color", vertex_shader_text, fragment_shader_text);

        auto verts =
            std::vector<viz::draw::Vec2>{Vec2(-0.6f, -0.4f), Vec2(0.6f, -0.4f), Vec2(0.0f, 0.6f)};
        auto pipeline = viz::draw::Pipeline::Create("color", verts);
        renderables_.emplace_back(std::make_shared<viz::draw::ExplicitRenderable>(
            std::move(pipeline), GL_TRIANGLES, viz::draw::Colors::grass));
        renderables_.emplace_back(
            viz::draw::Create(viz::draw::LineSegment(Vec3(-1.f, 0.f, 0.f), Vec3(1.f, 0.f, 0.f)),
                              viz::draw::Colors::black));

        auto window_size = app->WindowSize();
        auto aspect = float(window_size[0]) / float(window_size[1]);
        camera = viz::draw::Camera::Default(aspect);

        auto circle = viz::draw::Circle(Vec3(0.f, 0.f, 0.f), 1.f, Vec3(0.f, 0.f, 1.f));
        renderables_.emplace_back(viz::draw::Create(circle, viz::draw::Colors::orange));
    }

    void Key(App* app, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        // std::cout << "KEY\n";
    }

    void Mouse(App* app, int32_t button, int32_t action, int32_t mods)
    {
        mouse_down = true;
        mouse_drag = true;
    }

    void Cursor(App* app, double xpos, double ypos)
    {
        auto window_size = app->WindowSize();
        auto mouse = viz::draw::Vec2((float(xpos) / float(window_size[0])) * 2.f - 1.f,
                                     (float(ypos) / float(window_size[0])) * 2.f - 1.f);
        if (mouse_down) {
            trackball.Begin(mouse);
        }
        else if (mouse_drag) {
            trackball.Next(mouse, camera);
        }
        mouse_down = false;
    }

    void Draw(App* app)
    {
        auto window_size = app->WindowSize();
        glViewport(0, 0, window_size[0], window_size[1]);
        glClearColor(0.93f, 0.93f, 0.93f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        viz::draw::Context::Get().ApplyCamera(camera);
        for (auto& renderable : renderables_) {
            renderable->Draw();
        }
    }

    std::vector<viz::draw::Renderable::Ref> renderables_;
    viz::draw::Camera camera;
    viz::draw::Trackball trackball;
    bool mouse_down;
    bool mouse_drag;
};

int main(void) { return C1::App::Create(C1(), "C1", 640, 480)->Run(); }
