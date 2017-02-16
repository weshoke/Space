#include "renderable_factory.h"
#include "geom/primitives.h"
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

        Renderable::Ref Create(const LineSegment &line_segment, uint32_t color)
        {
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", line_segment.points()), GL_LINES, color);
        }

        Renderable::Ref Create(const Circle &circle, uint32_t color)
        {
            auto N = 90u;
            auto theta = M_PI * 2.f / float(N);
            auto p = space::geom::PerpendicularAxis(circle.normal()) * circle.radius();
            auto rot = E3f::AxisAngle(circle.normal(), theta);
            auto points = std::vector<Vec3>();
            points.reserve(N);
            for (auto i = 0u; i < N; ++i) {
                points.push_back(p);
                p = p.Spin(rot);
            }
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", points), GL_LINE_LOOP, color);
        }

        Renderable::Ref Create(const space::geom::Sphere<Vec3> &sphere, uint32_t color, float tol)
        {
            auto points = std::vector<Vec3>();
            auto res = 20u;
            points.reserve(res * res);

            for (auto j = 0u; j <= res; ++j) {
                auto v = float(j) / float(res);
                for (auto i = 0u; i < res; ++i) {
                    auto u = float(i) / float(res);
                    points.emplace_back(sphere.Point(u, v));
                }
            }

            const auto idx = [res](auto i, auto j) { return (i % res) + (j % res) * res; };

            auto index = std::vector<uint32_t>();
            index.reserve(2 * (res + 1) * (res + 1));
            for (auto j = 0u; j <= res; ++j) {
                for (auto i = 0u; i <= res; ++i) {
                    index.push_back(idx(i, j));
                    index.push_back(idx(i, j + 1));
                }
            }
            return std::make_shared<ExplicitRenderable>(
                Pipeline::Create("color", points, index), GL_TRIANGLE_STRIP, color);
        }
    }
}
