#version 410

uniform mat4 MVP;
in vec4 pos;

out vec2 coord;

void main() {
    gl_Position = MVP * pos;
    coord = gl_Position.xy;
}
