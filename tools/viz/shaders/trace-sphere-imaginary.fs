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

bool Xor(bool a, bool b)
{
    return bool(int(a) + int(b) & 1);
}

float Param(vec3 p)
{
    float ds = 0.005;
    float phi = (atan(p.y, p.x) + PI) / ( 2.0 * PI);
    float x = Saw(phi, 40.);
    float y = Saw(abs(p.z), 8.);
    phi = sqrt(x * x + y * y);
    phi = 1. - smoothstep(0.4, 0.4 + ds, phi);
    phi *= float(abs(p.z) < 0.84);
    phi += float(abs(p.z) > 0.9 && abs(p.z) < 0.95);
    phi += float(abs(p.z) > 0.99);
    return phi;
}

void main()
{
    Ray ray = CameraRay(view_matrix, world_pos);

    vec2 t = IntersectSphere(ray, center, radius);
    if(t.y < 0.)
    {
        discard;
    }
    vec3 p_front = (RayPoint(ray, t.x) - center) / radius;
    float res_front = Param(p_front);
    if(res_front <= 0)
    {
        discard;
    }

    pixel = color;
    pixel.a = res_front;
}
