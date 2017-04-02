#ifndef RAY_GLSL
#define RAY_GLSL

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

Ray CameraRay(mat4 view_matrix, vec3 world_pos) {
    mat3 view_matrix3 = mat3(view_matrix);
    vec3 eye = -(view_matrix[3].xyz) * view_matrix3;
    vec3 view_dir = world_pos - eye;
    return Ray(eye, normalize(view_dir));
}

#endif
