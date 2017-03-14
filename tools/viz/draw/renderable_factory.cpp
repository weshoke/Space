#include "renderable_factory.h"
#include "geom/primitives.h"
#include "mesh_factory.h"
#include "pipeline.h"
#include "primitives.h"
#include <memory>

namespace viz {
    namespace draw {
        Renderable::Ref CreateGrid2d(const Vec3 &start,
                                     const Vec3 &end,
                                     uint32_t divisions,
                                     uint32_t color)
        {
            auto points = std::vector<Vec3>();
            points.reserve((divisions + 1) * 2);

            auto step_s = (end[0] - start[0]) / float(divisions);
            for (auto s = 0u; s <= divisions; ++s) {
                auto coord_s = start[0] + step_s * float(s);
                points.push_back(Vec3(coord_s, start[1], start[2]));
                points.push_back(Vec3(coord_s, end[1], start[2]));
            }

            auto step_t = (end[1] - start[1]) / float(divisions);
            for (auto t = 0u; t <= divisions; ++t) {
                auto coord_t = start[1] + step_t * float(t);
                points.push_back(Vec3(start[0], coord_t, start[2]));
                points.push_back(Vec3(end[0], coord_t, start[2]));
            }

            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", points), GL_LINES, color);
        }

        Renderable::Ref CreateAxes(float length, uint32_t color)
        {
            auto points = std::vector<Vec3>{Vec3(-length, 0.f, 0.f),
                                            Vec3(length, 0.f, 0.f),
                                            Vec3(0.f, -length, 0.f),
                                            Vec3(0.f, length, 0.f),
                                            Vec3(0.f, 0.f, -length),
                                            Vec3(0.f, 0.f, length)};
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", points), GL_LINES, color);
        }

        Renderable::Ref Create(const Circle2d &circle, uint32_t color)
        {
            const auto &center = circle.center();
            return Create(
                Circle(Vec3(center[0], center[1], 0.f), circle.radius(), Vec3(0.f, 0.f, 1.f)),
                color);
        }

        Renderable::Ref Create(const Circle &circle, uint32_t color)
        {
            auto N = 90u;
            auto theta = M_PI * 2.f / float(N);
            auto p = space::geom::PerpendicularAxis(circle.normal()) * circle.radius();
            auto rot = E3::AxisAngle(circle.normal(), theta);
            auto points = std::vector<Vec3>();
            points.reserve(N);
            for (auto i = 0u; i < N; ++i) {
                points.push_back(p + circle.center());
                p = p.Spin(rot);
            }
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", points), GL_LINE_LOOP, color);
        }

        namespace {

            Renderable::Ref Create(const space::geom::Sphere<Vec3> &sphere,
                                   uint32_t color,
                                   const std::string &program)
            {
                return Create(
                    CreateIcosohedron(),
                    color,
                    program,
                    GL_TRIANGLES,
                    Matrix4::Translate(sphere.center()),
                    UniformMap().Add("center", sphere.center()).Add("radius", sphere.radius()));
            }

            std::string SphereProgram(float radius)
            {
                return std::abs(radius) < 1e-3f
                           ? "trace-point"
                           : (radius > 0.f ? "trace-sphere-real" : "trace-sphere-imaginary");
            }
        }

        Renderable::Ref Create(const Point2d &p, uint32_t color)
        {
            auto radius = C2::Round::Radius(p);
            auto center = C2::Round::CenterPoint(p);
            return Create(Sphere(Vec3(center[0], center[1], 0.f), std::abs(radius)),
                          color,
                          SphereProgram(radius));
        }

        Renderable::Ref Create(const Point &p, uint32_t color)
        {
            auto radius = C3::Round::Radius(p);
            auto center = C3::Round::CenterPoint(p);
            return Create(Sphere(Vec3(center[0], center[1], center[2]), std::abs(radius)),
                          color,
                          SphereProgram(radius));
        }

        Renderable::Ref Create(const Vec2 &p, uint32_t color, float radius)
        {
            return Create(Sphere(Vec3(p[0], p[1], 0.f), radius), color);
        }

        Renderable::Ref Create(const Vec3 &p, uint32_t color, float radius)
        {
            return Create(Sphere(p, radius), color);
        }

        Renderable::Ref Create(const space::geom::Sphere<Vec3> &sphere, uint32_t color)
        {
            return Create(sphere, color, "trace-point");
        }

        Renderable::Ref Create(Mesh &&mesh,
                               uint32_t color,
                               const std::string &program,
                               GLenum primitve,
                               Matrix4 model,
                               UniformMap uniforms)
        {
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create(program, std::move(mesh)), primitve, color, model, uniforms);
        }
    }
}
