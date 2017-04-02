#version 410

uniform mat4 view_matrix;
uniform vec3 center;
uniform float radius;
uniform vec4 color;

in vec3 world_pos;
out vec4 pixel;

#include "ray.glsl"
#include "sphere.glsl"

#define PI 3.14159265359

float Saw(float t, float N)
{
    return abs(mod(t, 1. / N) - 0.5 / N) * N * 2.;
}

void main()
{
    Ray ray = CameraRay(view_matrix, world_pos);
    float t = IntersectSphere(ray, center, radius).x;
    if(t < 0.)
    {
        discard;
    }
    vec3 p = (RayPoint(ray, t) - center) / radius;
    float ds = 0.005;
    float theta = smoothstep(-ds, ds, p.x) + smoothstep(-ds, ds, p.y);
    theta = 1. - abs(theta - 1.);
    theta += smoothstep(-ds, ds, p.z);
    theta = 1. - abs(theta - 1.);
    pixel = vec4(vec3(theta), 1.0) * color;
}
