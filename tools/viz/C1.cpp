#include "algebras/C1.h"
#include "algebras/C2.h"
#include "app/app.h"
#include "app/trackball_state.h"
#include "dbg/type.h"
#include "draw/camera.h"
#include "draw/color.h"
#include "draw/draw.h"
#include "draw/mesh_factory.h"
#include "draw/pipeline.h"
#include "draw/renderable.h"
#include "draw/renderable_factory.h"
#include "draw/shader_preprocessor.h"
#include "draw/surface.h"
#include "draw/trackball.h"
#include "filesystem/path.h"
#include "geom/primitives.h"
#include <sstream>

auto shader_dir = filesystem::path("../../tools/viz/shaders").make_absolute();

using C1 = space::algebras::C1<float>;
using C2 = space::algebras::C2<float>;
using C3 = space::algebras::C3<float>;

class C1Viz {
   public:
    C1Viz(viz::app::App& app)
    : app_(app)
    , tests_mask_(0xFF)
    , camera(viz::draw::Camera::Default())
    , trackball(viz::draw::Trackball(1.3f, 1.f, 10.f))
    , surface(viz::draw::Surface::Create(4, 4))
    {
        Init();
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

    void Init()
    {
        using Vec2 = viz::draw::Vec2;
        using Vec3 = viz::draw::Vec3;
        tests_.emplace(0u, std::vector<viz::draw::Renderable::Ref>());
        tests_.emplace(1u, std::vector<viz::draw::Renderable::Ref>());
        tests_.emplace(2u, std::vector<viz::draw::Renderable::Ref>());

        app().AddSearchPath(shader_dir);

        std::cout << glGetString(GL_VERSION) << "\n";
        app().LoadProgram("color");
        app().LoadProgram("color-texture");
        //        app().LoadProgram("trace-point");
        //        app().LoadProgram("trace-sphere-real");
        //        app().LoadProgram("trace-sphere-imaginary");
        //		app().LoadProgram("trace-rotor");
        //		app().LoadProgram("wireframe");

        auto window_size = app().WindowSize();
        auto aspect = float(window_size[0]) / float(window_size[1]);
        camera = viz::draw::Camera(
            Vec3(0.f, 0.f, 6.f), Vec3(0.f, 0.f, -3.f), Vec3(0.f, 1.f, 0.f), 35.f, aspect);

        // auto p1 = C3::Round::DualSphere(C3::EVec(-1.f, 0.f, 0.f), 1.f);
        // auto p2 = C3::Round::DualSphere(C3::EVec(1.f, 0.f, 0.f), -1.f);
        //        renderables_.emplace_back(viz::draw::Create(p1, viz::draw::Colors::red));
        //         renderables_.emplace_back(viz::draw::Create(p2, viz::draw::Colors::red));

        const auto cylinder_distance = [](
            Vec3 p, Vec3 center, Vec3 axis, float radius, float height) {
            auto from_center = p - center;
            auto xx = Vec2(Vec2(from_center[0], from_center[1]).Norm(), from_center[2]);
            xx[0] = std::abs(xx[0]);
            xx[1] = std::abs(xx[1]);
            Vec2 d = xx - Vec2(radius, height);
            d[0] = std::max(d[0], 0.f);
            d[1] = std::max(d[1], 0.f);
            return std::min(std::max(d[0], d[1]), 0.f) + d.Norm();
        };

        auto qp = std::vector<Vec3>{
            Vec3(-1.f, -1.f, 0.f), Vec3(1.f, -1.f, 0.f), Vec3(1.f, 1.f, 0.f), Vec3(-1.f, 1.f, 0.f),
        };
        auto qtc = std::vector<Vec2>{
            Vec2(0.f, 0.f), Vec2(1.f, 0.f), Vec2(1.f, 1.f), Vec2(0.f, 1.f),
        };
        auto qi = std::vector<uint32_t>{0, 3, 1, 1, 3, 2};
        auto image = std::vector<float>();
        for (auto i = 0; i < 16; ++i) {
            image.push_back(float(i) / 15.f);
        }
        auto tex = std::make_shared<viz::gl::Texture>();
        viz::gl::Error("tex");
        tex->Bind()
            .ImageData(GL_R32F, GL_RED, 4, 4, image)
            .Parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            .Parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        auto mesh = viz::draw::Mesh();
        mesh.Bind().Vertex(qp).TexCoord(qtc).Index(qi);
        tests_[0].emplace_back(viz::draw::Create(std::move(mesh),
                                                 viz::draw::Colors::grey,
                                                 "color-texture",
                                                 GL_TRIANGLES,
                                                 viz::draw::Matrix4::Identity(),
                                                 viz::draw::UniformMap().Add("tex", 0),
                                                 {tex}));

        //		auto N = 2;
        //		for(auto j = -N; j <= N; ++j)
        //		{
        //			for(auto i = -N; i <= N; ++i)
        //			{
        //				auto p = Vec3(-0.5f * i, -0.5f * j, -6.f) + camera.eye();
        //				auto ray = camera.ViewRay(p);
        //				renderables_.emplace_back(viz::draw::Create(ray, viz::draw::Colors::black,
        // 10.f));
        //				auto t = 0.f;
        ////				std::cout << "\n\n";
        //				auto count = 0;
        //				auto d = cylinder_distance(ray.Point(t), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f,
        // 1.f),
        // 1.f, 0.1f);
        ////				std::cout << d << "\n";
        //				while(d > 1e-2f && count < 3)
        //				{
        //					t += d;
        //					d = cylinder_distance(ray.Point(t), Vec3(0.f, 0.f, 0.f), Vec3(0.f, 0.f,
        // 1.f),
        // 1.f, 0.1f);
        ////					std::cout << d << "\n";
        //					++count;
        //					renderables_.emplace_back(viz::draw::Create(ray.Point(t),
        // viz::draw::Colors::sky));
        //				}
        //			}
        //		}
        //
        //		renderables_.emplace_back(viz::draw::Create(camera.eye(), viz::draw::Colors::red));
        //
        //		tests_[0].emplace_back(viz::draw::Create(viz::draw::CreateExtrudedPolygon(60, 0.1f),
        //                                                            viz::draw::Colors::red,
        //                                                            "wireframe",
        //                                                            GL_TRIANGLES));

        //		renderables_.emplace_back(viz::draw::Create(viz::draw::CreateExtrudedPolygon(6),
        //													viz::draw::Colors::red,
        //													"trace-rotor",
        //													GL_TRIANGLES,
        //													viz::draw::Matrix4::Identity(),
        //													viz::draw::UniformMap()
        //														.Add("center", Vec3(0.f, 0.f, 0.f))
        //														.Add("axis", Vec3(0.f, 0.f, 1.f))
        //														.Add("radius", 1.f)));

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

    void Key(int32_t key, int32_t scancode, int32_t action, int32_t mods)
    {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_1:
                    tests_mask_ ^= 1 << (key - GLFW_KEY_1);
                    break;
            }
        }
    }

    void Mouse(int32_t button, int32_t action, int32_t mods)
    {
        trackball_state.Mouse(button, action, mods);
    }

    void Cursor(double xpos, double ypos)
    {
        using Vec2 = viz::draw::Vec2;
        auto window_size = app().WindowSize();
        auto mouse_pos = Vec2((float(xpos) / float(window_size[0])) * 2.f - 1.f,
                              (float(ypos) / float(window_size[0])) * 2.f - 1.f);

        trackball_state.Cursor(camera, trackball, mouse_pos);
    }

    void Scroll(int xoffset, int yoffset) { trackball.Move(camera, float(yoffset) * 0.2f); }
    void Draw()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        auto window_size = app().WindowSize();
        viz::draw::ClearWindowWithDepth(window_size, viz::draw::Colors::chromium);

        using Vec2 = viz::draw::Vec2;
        viz::draw::Context::Get().ApplyCamera(camera);
        viz::draw::Context::Get().ScreenSize(Vec2(float(window_size[0]), float(window_size[1])));

        const auto draw_renderables = [](auto& renderables) {
            for (auto& renderable : renderables) {
                renderable->Draw();
            }
        };
        draw_renderables(renderables_);
        for (auto i = 0u; i < 3; ++i) {
            if (tests_mask_ & (1 << i)) {
                draw_renderables(tests_[i]);
            }
        }
    }

    viz::app::App& app() { return app_; }
    viz::app::App& app_;
    std::vector<viz::draw::Renderable::Ref> renderables_;
    uint8_t tests_mask_;
    std::unordered_map<uint32_t, std::vector<viz::draw::Renderable::Ref>> tests_;
    viz::draw::Camera camera;
    viz::draw::Trackball trackball;
    viz::draw::Surface surface;
    viz::app::TrackballState trackball_state;
};

int main(void)
{
    return viz::app::App::Create("C1", 640, 480, [](viz::app::App& app) {
        auto c1 = C1Viz(app);
        return app.Run(c1);
    });
}
