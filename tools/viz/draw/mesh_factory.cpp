#include "mesh_factory.h"
#include "primitives.h"
#include <cmath>
#include <vector>

namespace viz {
    namespace draw {
        Mesh CreateIcosohedron()
        {
            // TODO: scale in shader?
            auto t = (1.f + std::sqrt(5.f)) * 0.5f;
            // TODO: radius of inscribed sphere
            auto s = 0.75f;  // 1.f / std::sqrt(1.f + t * t);
            // clang-format off
            auto vertex = std::vector<Vec3>{
                Vec3(-1.f, t, 0.f) * s,
                Vec3(1.f, t, 0.f) * s,
                Vec3(-1.f, -t, 0.f) * s,
                Vec3(1.f, -t, 0.f) * s,

                Vec3(0.f, -1.f, t) * s,
                Vec3(0.f, 1.f, t) * s,
                Vec3(0.f, -1.f, -t) * s,
                Vec3(0.f, 1.f, -t) * s,

                Vec3(t, 0.f, -1.f) * s,
                Vec3(t, 0.f, 1.f) * s,
                Vec3(-t, 0.f, -1.f) * s,
                Vec3(-t, 0.f, 1.f) * s};

            auto index = std::vector<uint32_t>{
                // 5 faces around point 0
                0, 11, 5,
                0, 5, 1,
                0, 1, 7,
                0, 7, 10,
                0, 10, 11,

                // 5 adjacent faces
                1, 5, 9,
                5, 11, 4,
                11, 10, 2,
                10, 7, 6,
                7, 1, 8,

                // 5 faces around point 3
                3, 9, 4,
                3, 4, 2,
                3, 2, 6,
                3, 6, 8,
                3, 8, 9,

                // 5 adjacent faces
                4, 9, 5,
                2, 4, 11,
                6, 2, 10,
                8, 6, 7,
                9, 8, 1
            };
            // clang-format on
            auto mesh = Mesh();
            mesh.Bind().Vertex(vertex).Index(index);
            return mesh;
        }

        Mesh CreateExtrudedPolygon(uint32_t sides)
        {
            auto theta = M_PI * 2.f / float(sides);
            auto ct = std::cos(theta);
            auto st = std::sin(theta);
            auto p = Vec3(1.f, 0.f, 0.f);
            auto vertex = std::vector<Vec3>();
            vertex.reserve(sides * 2);
            for (auto i = 0u; i < sides; ++i) {
                vertex.push_back(p);
                vertex.push_back(Vec3(p[0], p[1], 1.f));
                auto x = p[0];
                auto y = p[1];
                p[0] = ct * x - st * y;
                p[1] = ct * y + st * x;
            }
            auto index = std::vector<uint32_t>();
            index.reserve(12 * (sides - 1));
            for (auto i = 0u; i < sides - 1; ++i) {
                index.push_back(2 * i);
                index.push_back(2 * (i + 1));
                index.push_back(2 * i + 1);

                index.push_back(2 * (i + 1));
                index.push_back(2 * (i + 1) + 1);
                index.push_back(2 * i + 1);
            }
            {
                index.push_back(2 * (sides - 1));
                index.push_back(0);
                index.push_back(2 * (sides - 1) + 1);

                index.push_back(0);
                index.push_back(1);
                index.push_back(2 * (sides - 1) + 1);
            }
            for (auto i = 0; i < sides - 2; ++i) {
                index.push_back(0);
                index.push_back(2 * (i + 1));
                index.push_back(2 * (i + 2));
                index.push_back(1);
                index.push_back(2 * (i + 2) + 1);
                index.push_back(2 * (i + 1) + 1);
            }

            auto mesh = Mesh();
            mesh.Bind().Vertex(vertex).Index(index);
            return mesh;
        }
    }
}
