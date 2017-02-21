#include "algebra/C1.h"
#include "app/app.h"
#include "draw/camera.h"
#include "draw/color.h"
#include "draw/draw.h"
#include "draw/pipeline.h"
#include "draw/renderable.h"
#include "draw/renderable_factory.h"
#include "draw/trackball.h"
#include "geom/primitives.h"
#include "mpark/variant.hpp"
#include "round.h"
#include <sstream>

static const char* vertex_shader_text =
    "#version 410\n"
    "uniform mat4 MVP;\n"
    "in vec3 pos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(pos, 1.0);\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 410\n"
    "uniform vec4 color;\n"
    "out vec4 pixel;\n"
    "void main()\n"
    "{\n"
    "    pixel = color;\n"
    "}\n";

struct MouseEventState {
   public:
    struct MousePressed {
    };
    struct MouseDown {
    };
    struct MouseUp {
    };
    struct MouseDrag {
    };
    struct MouseDragging {
    };
    using State = mpark::variant<MousePressed, MouseDown, MouseUp, MouseDrag, MouseDragging>;

    MouseEventState()
    : state_(MouseUp())
    {
    }

    void ButtonPressed() { state_ = MousePressed(); };
    void Cursor()
    {
        if (ButtonWasPressed()) {
            state_ = MouseDown();
        }
        else if (DragWasStarted()) {
            state_ = MouseDragging();
        }
    }
    void ButtonReleased() { state_ = MouseUp(); };
    void DragStarted() { state_ = MouseDrag(); }
    bool ButtonWasPressed() { return mpark::holds_alternative<MousePressed>(state_); }
    bool IsButtonDown() { return state_.index() <= 1; }
    bool DragWasStarted() { return mpark::holds_alternative<MouseDrag>(state_); }
    bool Dragging() { return state_.index() >= 3; }
   private:
    State state_;
};

using C1 = space::algebra::C1<float>;

// TODO: should be nested contexts
// auto app = App {
//      auto c1 = C1Viz {
//
//          return
//      }
// }
class C1Viz {
   public:
    using App = app::App<C1Viz>;

    C1Viz()
    : camera(viz::draw::Camera::Default())
    , trackball(viz::draw::Trackball(1.3f, 1.f, 5.f))
    {
    }

    void Init(App* app)
    {
        using Vec2 = viz::draw::Vec2;
        using Vec3 = viz::draw::Vec3;
        std::cout << glGetString(GL_VERSION) << "\n";
        viz::draw::Context::Get().RegisterProgram(
            "color", vertex_shader_text, fragment_shader_text);

        auto verts =
            std::vector<viz::draw::Vec2>{Vec2(-0.6f, -0.4f), Vec2(0.6f, -0.4f), Vec2(0.0f, 0.6f)};
        auto pipeline = viz::draw::Pipeline::Create("color", verts);

        auto window_size = app->WindowSize();
        auto aspect = float(window_size[0]) / float(window_size[1]);
        camera = viz::draw::Camera::Default(aspect);

        renderables_.emplace_back(viz::draw::CreateAxes(2.f, viz::draw::Colors::grey));
        renderables_.emplace_back(viz::draw::CreateGrid2d(
            Vec3(-5.f, 1.f, -5.f), Vec3(5.f, 1.f, 5.f), 10, viz::draw::Colors::grey));

        auto horosphere = std::vector<Vec3>();
        horosphere.reserve(101);
        for (auto i = 0u; i <= 100u; ++i) {
            auto x = float(i / 100.f) * 7.f - 7.f * 0.5f;
            auto y = 0.5f * x * x;
            horosphere.push_back(Vec3(x, 1.f, y));
        }
        renderables_.emplace_back(
            viz::draw::Create(horosphere, GL_LINE_STRIP, viz::draw::Colors::black));

        const auto map_range = [](auto v) { return float(v) / 20.f * 10.f - 5.f; };
        for (auto k = 0u; k <= 20u; ++k) {
            //			for(auto j = 5u; j <= 15u; j += 3)
            auto j = 8.5f;
            {
                for (auto i = 0u; i <= 20u; ++i) {
                    auto e1 = map_range(i);
                    auto no = map_range(j);
                    auto ni = map_range(k);
                    auto p = C1::Pnt(e1, no, ni);
                    auto size = space::round::Size(p);
                    auto r = std::sqrt(std::abs(size));
                    auto circle = viz::draw::Circle(
                        Vec3(e1, no, ni), std::max(r * 0.05f, 0.02f), Vec3(0.f, 1.f, 0.f));
                    auto c = size < 0.f ? viz::draw::Colors::sky : viz::draw::Colors::red;
                    renderables_.emplace_back(viz::draw::Create(circle, c));
                }
            }
        }
        // as the weight (no) coordinate decreases to 0, the amount of ni required to balance
        // the involute product increases toward infinity.  Once the coordinate crosses zero,
        // the graph of dual sphere sizes flips around the e1^no plane
        auto pp = C1::Pnt(0.f, 1.f, 1.f);
        auto pp_d = pp.Dual();
        std::cout << pp << "\n";
        auto ss = space::round::Size(pp_d);
        std::cout << pp_d << " " << ss << "\n";
    }

    void Key(App* app, int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        // std::cout << "KEY\n";
    }

    // GLFW_MOD_SHIFT
    // GLFW_MOD_CONTROL
    // GLFW_MOD_ALT
    // GLFW_MOD_SUPER

    void Mouse(App* app, int32_t button, int32_t action, int32_t mods)
    {
        if (mods == GLFW_MOD_ALT) {
            action == 1 ? mouse.DragStarted() : mouse.ButtonReleased();
        }
        else if (mods == 0) {
            action == 1 ? mouse.ButtonPressed() : mouse.ButtonReleased();
        }
        else if (action == 0) {
            mouse.ButtonReleased();
        }
    }

    void Cursor(App* app, double xpos, double ypos)
    {
        const auto mouse_pos = [&]() {
            auto window_size = app->WindowSize();
            return viz::draw::Vec2((float(xpos) / float(window_size[0])) * 2.f - 1.f,
                                   (float(ypos) / float(window_size[0])) * 2.f - 1.f);
        };

        if (mouse.IsButtonDown()) {
            if (mouse.ButtonWasPressed()) {
                trackball.Begin(camera, mouse_pos());
            }
            else {
                camera = trackball.Next(mouse_pos());
            }
        }
        else if (mouse.Dragging()) {
            if (mouse.DragWasStarted()) {
                trackball.Begin(camera, mouse_pos());
            }
            else {
                camera = trackball.Pan(mouse_pos());
            }
        }
        mouse.Cursor();
    }

    void Scroll(App* app, int xoffset, int yoffset)
    {
        trackball.Move(camera, float(yoffset) * 0.2f);
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
    MouseEventState mouse;
};

int main(void) { return C1Viz::App::Create(C1Viz(), "C1", 640, 480)->Run(); }
