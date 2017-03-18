#version 410

uniform mat4 MVP;
uniform mat4 model_matrix;
uniform float radius;
in vec3 pos;

out vec3 world_pos;

#define TWO_OVER_SQRT_3 1.155

void main() {
    float s = TWO_OVER_SQRT_3 * radius;
    vec4 p = vec4(vec3(pos.xy * s, pos.z), 1.0);
    gl_Position = MVP * p;
    world_pos = (model_matrix * p).xyz;
}
