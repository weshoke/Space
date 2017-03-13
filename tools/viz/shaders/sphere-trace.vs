#version 410

uniform mat4 MVP;
uniform mat4 model_matrix;
in vec4 pos;

out vec3 world_pos;

void main() {
    gl_Position = MVP * pos;
    world_pos = (model_matrix * pos).xyz;
}
