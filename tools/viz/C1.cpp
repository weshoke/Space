#include "algebras/C1.h"
#include "algebras/C2.h"
#include "app/app.h"
#include "dbg/type.h"
#include "draw/camera.h"
#include "draw/color.h"
#include "draw/draw.h"
#include "draw/mesh_factory.h"
#include "draw/pipeline.h"
#include "draw/renderable.h"
#include "draw/renderable_factory.h"
#include "draw/trackball.h"
#include "filesystem/path.h"
#include "geom/primitives.h"
#include "mpark/variant.hpp"
#include <sstream>

auto shader_dir = filesystem::path("../../tools/viz/shaders").make_absolute();

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

using C1 = space::algebras::C1<float>;
using C2 = space::algebras::C2<float>;

// TODO: should be nested contexts
// TODO: use lambda and pass App into it
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
    , trackball(viz::draw::Trackball(1.3f, 1.f, 10.f))
    {
    }

    void MakeGrid()
    {
        using Vec3 = viz::draw::Vec3;
        renderables_.emplace_back(viz::draw::CreateAxes(2.f, viz::draw::Colors::grey));
        //        renderables_.emplace_back(viz::draw::CreateGrid2d(
        //            Vec3(-5.f, 1.f, -5.f), Vec3(5.f, 1.f, 5.f), 10, viz::draw::Colors::grey));
    }

    void MakeHorosphere()
    {
        using Vec3 = viz::draw::Vec3;
        auto horosphere = std::vector<Vec3>();
        horosphere.reserve(101);
        for (auto i = 0u; i <= 100u; ++i) {
            auto x = float(i / 100.f) * 7.f - 7.f * 0.5f;
            auto y = 0.5f * x * x;
            horosphere.push_back(Vec3(x, 1.f, y));
        }
        renderables_.emplace_back(
            viz::draw::Create(horosphere, GL_LINE_STRIP, viz::draw::Colors::black));
    }

    void MakeSizeCircles(float no)
    {
        //        using Vec3 = viz::draw::Vec3;
        //        const auto map_range = [](auto v) { return float(v) / 20.f * 10.f - 5.f; };
        //        for (auto k = 0u; k <= 20u; ++k) {
        //            for (auto i = 0u; i <= 20u; ++i) {
        //                auto e1 = map_range(i);
        //                auto ni = map_range(k);
        //                auto p = C1::Pnt(e1, no, ni);
        //                auto size = space::round::Size(p);
        //                auto r = std::sqrt(std::abs(size));
        //                auto circle = viz::draw::Circle(
        //                    Vec3(e1, no, ni), std::max(r * 0.05f, 0.02f), Vec3(0.f, 1.f, 0.f));
        //                auto c = size < 0.f ? viz::draw::Colors::sky : viz::draw::Colors::red;
        //                renderables_.emplace_back(viz::draw::Create(circle, c));
        //            }
        //        }
    }

    void VisualizeSize()
    {
        MakeGrid();
        MakeHorosphere();
        MakeSizeCircles(1.f);
    }

    void VisualizeC1()
    {
        using Vec3 = viz::draw::Vec3;

        // Dual of sphere
        auto s = C1::Round::DualSphere(C1::EVec(0.f), 1.f);
        auto pss = C1::Pss(1.f);
        auto s_ = s <= pss;
        auto s_pts = C1::Round::Split(s_);

        // Spherical inversion of point
        auto p1 = C1::Round::Point(C1::E1(0.5f));
        auto p2 = p1.Spin(s);
        p2 = p2 / p2[1];

        renderables_.emplace_back(
            viz::draw::Create(viz::draw::Circle(Vec3(s[0], 0.f, 0.f), 1.f, Vec3(0.f, 0.f, 1.f)),
                              viz::draw::Colors::sky));
        renderables_.emplace_back(
            viz::draw::Create(Vec3(s_pts[0][0], 0.f, 0.f), viz::draw::Colors::red));
        renderables_.emplace_back(
            viz::draw::Create(Vec3(s_pts[1][0], 0.f, 0.f), viz::draw::Colors::red));

        renderables_.emplace_back(
            viz::draw::Create(Vec3(p1[0], 0.f, 0.f), viz::draw::Colors::yellow));
        renderables_.emplace_back(
            viz::draw::Create(Vec3(p2[0], 0.f, 0.f), viz::draw::Colors::grass));
    }

    void Init(App* app)
    {
        //        using Vec2 = viz::draw::Vec2;
        using Vec3 = viz::draw::Vec3;

        app->AddSearchPath(shader_dir);

        std::cout << glGetString(GL_VERSION) << "\n";
        viz::draw::Context::Get().RegisterProgram(
            "color", app->LoadFile("color.vs"), app->LoadFile("color.fs"));
        viz::draw::Context::Get().RegisterProgram(
            "sphere-trace", app->LoadFile("sphere-trace.vs"), app->LoadFile("sphere-trace.fs"));
        viz::draw::Context::Get().RegisterProgram("wireframe",
                                                  app->LoadFile("wireframe.vs"),
                                                  app->LoadFile("wireframe.gs"),
                                                  app->LoadFile("wireframe.fs"));

        auto window_size = app->WindowSize();
        auto aspect = float(window_size[0]) / float(window_size[1]);
        camera = viz::draw::Camera(
            Vec3(0.f, 0.f, 6.f), Vec3(0.f, 0.f, -3.f), Vec3(0.f, 1.f, 0.f), 35.f, aspect);

        renderables_.emplace_back(viz::draw::Create(
            viz::draw::CreateIcosohedron(), viz::draw::Colors::red, "sphere-trace", GL_TRIANGLES));

        // VisualizeC1();

        // DualPlane = Point <= FreeVector
        // Tangent = Point ^ DualPlane

        // auto p1 = C2::Round::Point(C2::EVec(1.f, 0.f));
        // auto p2 = C2::Round::Point(C2::EVec(1.f, 1.f));
        // auto p3 = C2::Round::Point(C2::EVec(-1.f, 0.f));
        // auto L = p1 ^ p2 ^ C2::Inf(1.f);
        // auto T = C2::Versor::Translator(C2::EVec(1.f, 0.f));
        // auto L2 = L.Spin(T);
        // auto R = C2::EVec(1.f, 0.f) * C2::EVec(1.f, 0.3f).Normalized();
        // auto L3 = L2.Spin((T * R * ~T));
        // //		std::cout << R << "\n";
        // //		std::cout << (T * R * ~T) << "\n";
        // renderables_.emplace_back(viz::draw::Create(viz::draw::Line2d(L),
        // viz::draw::Colors::red));
        // renderables_.emplace_back(viz::draw::Create(viz::draw::Line2d(L2),
        // viz::draw::Colors::sky));
        // renderables_.emplace_back(
        //     viz::draw::Create(viz::draw::Line2d(L3), viz::draw::Colors::orange));
        //
        // auto tang = p1 ^ (p1 <= (C2::EVec(-0.1f, 0.f) ^ C2::Inf(1.f)));
        // auto B = C2::Versor::Boost(tang);
        //
        // auto C = p1 ^ p2 ^ p3;
        //
        // renderables_.emplace_back(
        //     viz::draw::Create(viz::draw::Circle2d(C), viz::draw::Colors::red));
        // auto C2 = C;
        // for (auto i = 0u; i < 30u; ++i) {
        //     C2 = C2.Spin(B);
        //     renderables_.emplace_back(
        //         viz::draw::Create(viz::draw::Circle2d(C2), viz::draw::Colors::sky));
        // }
        // renderables_.emplace_back(viz::draw::Create(p1, viz::draw::Colors::black));
        // renderables_.emplace_back(viz::draw::Create(p2, viz::draw::Colors::black));
        // renderables_.emplace_back(viz::draw::Create(p3, viz::draw::Colors::black));

        MakeGrid();
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        using Vec2 = viz::draw::Vec2;
        viz::draw::Context::Get().ApplyCamera(camera);
        viz::draw::Context::Get().ScreenSize(Vec2(float(window_size[0]), float(window_size[1])));
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
