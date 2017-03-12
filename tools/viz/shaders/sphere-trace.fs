#version 410

uniform mat4 view_matrix;
uniform float fov_y_scale;
uniform float aspect;
uniform float z_near;
uniform float z_far;
// uniform vec4 color;

in vec2 coord;
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

vec3 CoordToModelDirection(vec2 c, float fov_y_scale, float aspect) {
    return vec3(c.x * fov_y_scale * aspect, c.y * fov_y_scale, -1.0);
}

Ray CameraRay(mat4 model_view_matrix, vec2 coord, float fov_y_scale, float aspect) {
    mat3 model_view_matrix3 = mat3(model_view_matrix);
    vec3 eye = -(model_view_matrix[3].xyz) * model_view_matrix3;
    vec3 model_dir = CoordToModelDirection(coord, fov_y_scale, aspect);
    vec3 view_dir = model_dir * model_view_matrix3;
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
        q = (-b - sqrt(disc))/2.0;
    }
    else
    {
        q = (-b + sqrt(disc))/2.0;
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

void main()
{
    vec3 center = vec3(0., 0., 0.);
    vec3 world_center = (view_matrix * vec4(center, 1.)).xyz;
    Ray ray = CameraRay(view_matrix, coord, fov_y_scale, aspect);
    float t = IntersectSphere(ray, world_center, 1.);
    pixel = vec4(ray.start, 1.0);
    pixel.rgb = vec3(t);
    // pixel.rgb = world_center;
    // pixel = vec4(coord, 0.0, 1.0);
    // pixel.b = aspect;
}
