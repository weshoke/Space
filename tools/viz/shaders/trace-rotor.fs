#version 410

uniform mat4 view_matrix;
uniform vec3 center;
uniform vec3 axis;
uniform float radius;
uniform vec4 color;

in vec3 world_pos;
out vec4 pixel;

struct Ray{
    vec3 start;
    vec3 dir;
};

#define PI 3.14159265359

vec3 RayPoint(Ray self, float t)
{
    return self.start + t * self.dir;
}

float RayParameter(Ray self, vec3 p)
{
    return dot(p - self.start, self.dir);
}

Ray CameraRay(mat4 view_matrix, vec3 model_pos) {
    mat3 view_matrix3 = mat3(view_matrix);
    vec3 eye = -(view_matrix[3].xyz) * view_matrix3;
    vec3 view_dir = world_pos - eye;
    return Ray(eye, normalize(view_dir));
}

vec2 IntersectInfiniteCylinder(Ray ray, vec3 center, vec3 axis, float radius)
{
    vec3 dp = ray.start - center;
    vec3 Av = ray.dir - dot(ray.dir, axis) * axis;
    vec3 Cv = dp - dot(dp, axis) * axis;
    float A = dot(Av, Av);
    float B = 2.0 * dot(Av, Cv);
    float C = dot(Cv, Cv) - radius * radius;
    float discriminant = B * B - 4.0 * A * C;
    if(discriminant < 0.)
    {
        return vec2(-1.0);
    }

    float Q = sqrt(discriminant);
    float t1 = (-B + Q) / (2.0 * A);
    float t2 = (-B - Q) / (2.0 * A);
    return t1 < t2 ? vec2(t1, t2) : vec2(t2, t1);
}

float RayPlaneIntersection(Ray ray, vec3 p, vec3 n)
{
    float align = dot(ray.dir, n);
    if(abs(align) < 1e-3f)
    {
        return -1.f;
    }
    return -(dot(ray.start, n) - dot(p, n)) / align;
}

float PlaneDistance(vec3 p, vec3 n, vec3 pt)
{
    return dot(n, pt - p);
}

float IntersectCylinderCap(Ray ray, vec3 center, vec3 axis, float radius, float height)
{
    vec3 center_top = center + axis * height;
    float t_top = RayPlaneIntersection(ray, center_top, axis);
    float t_bot = RayPlaneIntersection(ray, center, -axis);

    vec3 p_top = RayPoint(ray, t_top);
    vec3 p_bot = RayPoint(ray, t_bot);
    float d_top = length(p_top - center_top);
    float d_bot = length(p_bot - center);

    float t_min = 1e10;
    if(t_top >= 0. && d_top <= radius)
    {
        t_min = min(t_min, t_top);
    }
    if(t_bot >= 0. && d_bot <= radius)
    {
        t_min = min(t_min, t_bot);
    }
    return t_min == 1e10 ? -1.0 : t_min;
}

vec2 IntersectCylinder(Ray ray, vec3 center, vec3 axis, float radius, float height)
{
    vec2 t_inf = IntersectInfiniteCylinder(ray, center, axis, radius);
    if(t_inf.x < 0.)
    {
        return vec2(IntersectCylinderCap(ray, center, axis, radius, height), 1.);
    }
    vec3 p_inf = RayPoint(ray, t_inf.x);
    float d_top = PlaneDistance(center + axis * height, axis, p_inf);
    float d_bot = PlaneDistance(center, -axis, p_inf);
    if(d_top > 0. || d_bot > 0.)
    {
        return vec2(IntersectCylinderCap(ray, center, axis, radius, height), 1.);
    }

    float t_min = IntersectCylinderCap(ray, center, axis, radius, height);
    t_min = t_min == -1.0 ? t_inf.x : min(t_min, t_inf.x);
    return vec2(t_min, t_min == t_inf.x ? 0.0 : 1.0);
}

void main()
{
    Ray ray = CameraRay(view_matrix, world_pos);
    float height = 0.05;
    vec2 t = IntersectCylinder(ray, center, axis, radius, height);
    if(t.x < 0.)
    {
        discard;
    }

    vec2 t_inner = IntersectCylinder(ray, center, axis, radius - 0.1, height * 2.);
    if(t_inner.x >= 0)
    {
        discard;
    }

    vec3 p = RayPoint(ray, t.x);
    // vec3 p = world_pos;
    vec3 from = p - center;
    vec3 axis_aligned = dot(from, axis) * axis;
    float h = length(axis_aligned);
    float len = length(from);
    float r = sqrt(len * len - h * h);


    pixel.rgb = vec3(r, h, 0.);

    pixel.a = 1.;
}
