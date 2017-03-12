#version 410

uniform mat4 MVP;
in vec4 pos;

out vec3 world_pos;

void main() {
    gl_Position = MVP * pos;
    world_pos = pos.xyz;
}
