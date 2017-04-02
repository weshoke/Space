#version 410
uniform mat4 MVP;
in vec3 pos;
in vec2 tex_coord;
out vec2 tc;
void main()
{
    gl_Position = MVP * vec4(pos, 1.0);
    tc = tex_coord;
}
