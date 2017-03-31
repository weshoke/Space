#ifndef SPHERE_GLSL
#define SPHERE_GLSL

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

#endif
