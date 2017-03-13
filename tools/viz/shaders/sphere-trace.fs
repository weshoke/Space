#version 410

uniform mat4 view_matrix;
uniform vec3 center;

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

float IntersectSphere(Ray ray, vec3 center, float radius)
{
    vec3 oc = ray.start - center;
    float b = 2.0 * dot(ray.dir, oc);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - 4.0 * c;

    if (disc < 0.0)
    {
        return -1.0;
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
        return -1.0;
    }

    // if t0 is less than zero, the intersection point is at t1
    if (t0 < 0.0)
    {
        return t1;
    }
    else
    {
        return t0;
    }
}

#define PI 3.14159265359

float Saw(float t, float N)
{
    return abs(mod(t, 1. / N) - 0.5 / N) * N * 2.;
}

void main()
{
    Ray ray = CameraRay(view_matrix, world_pos);

    float t = IntersectSphere(ray, center, 1.);
    if(t < 0.)
    {
        discard;
    }
    vec3 p = RayPoint(ray, t) - center;
    float theta = (atan(p.y, p.x) + PI) / (2. * PI);
    float phi = acos(p.z) / PI;

    pixel.rgb = vec3(0.);
    float x = smoothstep(0.35, 0.45, Saw((phi + theta) / 2., 10.));
    pixel.rgb = vec3(x);
}
