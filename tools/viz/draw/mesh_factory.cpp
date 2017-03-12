#include "mesh_factory.h"
#include "primitives.h"
#include <cmath>
#include <vector>

namespace viz {
    namespace draw {
        Mesh CreateIcosohedron()
        {
            auto t = (1.f + std::sqrt(5.f)) * 0.5f;
            // clang-format off
            auto vertex = std::vector<Vec3>{
				Vec3(-1.f, t, 0.f),
				Vec3(1.f, t, 0.f),
				Vec3(-1.f, -t, 0.f),
				Vec3(1.f, -t, 0.f),

				Vec3(0.f, -1.f, t),
				Vec3(0.f, 1.f, t),
				Vec3(0.f, -1.f, -t),
				Vec3(0.f, 1.f, -t),

				Vec3(t, 0.f, -1.f),
				Vec3(t, 0.f, 1.f),
				Vec3(-t, 0.f, -1.f),
				Vec3(-t, 0.f, 1.f)};

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
    }
}
