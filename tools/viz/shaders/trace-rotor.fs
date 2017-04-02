#version 410

uniform mat4 view_matrix;
uniform vec3 center;
uniform vec3 axis;
uniform float radius;
uniform vec4 color;

in vec3 world_pos;
out vec4 pixel;

#include "ray.glsl"

#define PI 3.14159265359

// float sdCappedCylinder( vec3 p, vec2 h )
// {
//   vec2 d = abs(vec2(length(p.xz),p.y)) - h;
//   return min(max(d.x,d.y),0.0) + length(max(d,0.0));
// }


float CylinderDistance(vec3 p, vec3 center, vec3 axis, float radius, float height)
{
    vec3 from_center = p - center;
    vec2 d = abs(vec2(length(from_center.xy), from_center.z)) - vec2(radius, height);
    return min(max(d.x, d.y), 0.) + length(max(d, 0.));
}

vec3 CountColor(int count)
{
    if(count <= 2)
    {
        return vec3(1., 0., 0.);
    }
    else if(count <= 5)
    {
        return vec3(0., 1., 0.);
    }
    return vec3(0., 0., 1.);
}

const vec3 red = vec3(1., 0.2, 0.2);
const vec3 blue = vec3(0.2, 0.4, 1.);

void main()
{
    Ray ray = CameraRay(view_matrix, world_pos);
    float t = 0.;
    float d = CylinderDistance(RayPoint(ray, t), center, axis, radius, 0.1);
    int count = 0;
    float d_prev = d;
    while(abs(d) > 1e-2 && count <= 2)
    {
        t += d;
        d = CylinderDistance(RayPoint(ray, t), center, axis, radius, 0.1);
        if(d > 0. && d_prev > 0. && d > d_prev)
        {
            discard;
        }
        ++count;
        // break;
    }
    // pixel.rgb = CountColor(count);
    // discard;

    // float d = CylinderDistance(world_pos, center, axis, radius, 0.1);
    // pixel.rgb = vec3(d);
    // pixel.rgb = d < 0. ? vec3(0.4, -d, 0.) : vec3(0., d, 0.4);
    pixel.rgb = d > 0.1 ? red : blue;
    if(d < 0.05)
    {
        pixel.rgb = vec3(0.);
    }
    pixel.rgb = vec3(1.);
    // pixel.rgb = RayPoint(ray, t);

    // float height = 0.05;
    // vec2 t = IntersectCylinder(ray, center, axis, radius, height);
    // if(t.x < 0.)
    // {
    //     discard;
    // }
    //
    // vec2 t_inner = IntersectCylinder(ray, center, axis, radius - 0.1, height * 2.);
    // if(t_inner.x >= 0)
    // {
    //     discard;
    // }
    //
    // vec3 p = RayPoint(ray, t.x);
    // // vec3 p = world_pos;
    // vec3 from = p - center;
    // vec3 axis_aligned = dot(from, axis) * axis;
    // float h = length(axis_aligned);
    // float len = length(from);
    // float r = sqrt(len * len - h * h);
    //
    //
    // pixel.rgb = vec3(r, h, 0.);

    pixel.a = 1.;
}
