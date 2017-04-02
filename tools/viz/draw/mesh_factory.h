#ifndef SPACE_VIZ_DRAW_MESH_FACTORY_H
#define SPACE_VIZ_DRAW_MESH_FACTORY_H

#include "mesh.h"

namespace viz {
    namespace draw {
        Mesh CreateIcosohedron();
        Mesh CreateExtrudedPolygon(uint32_t sides);
        Mesh CreateExtrudedPolygon(uint32_t sides, float height);
    }
}

#endif
