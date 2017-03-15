#version 410

uniform mat4 view_matrix;
uniform vec3 center;
uniform float radius;
uniform vec4 color;

in vec3 world_pos;
out vec4 pixel;

struct Ray{
    vec3 start;
    vec3 dir;
};


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

vec2 IntersectSphere(Ray ray, vec3 center, float radius)
{
    vec3 oc = ray.start - center;
    float b = 2.0 * dot(ray.dir, oc);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - 4.0 * c;

    if (disc < 0.0)
    {
        return vec2(-1.0);
    }

    // compute q as described above
    float q;
    if (b < 0.0)
    {
        q = (-b - sqrt(disc)) / 2.0;
    }
    else
    {
        q = (-b + sqrt(disc)) / 2.0;
    }

    float t0 = q;
    float t1 = c / q;

    // make sure t0 is smaller than t1
    if (t0 > t1) {
        // if t0 is bigger than t1 swap them around
        float temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // if t1 is less than zero, the object is in the ray's negative direction
    // and consequently the ray misses the sphere
    if (t1 < 0.0)
    {
        return vec2(-1.0);
    }
    return vec2(t0, t1);
}

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
