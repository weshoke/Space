#version 410
uniform vec4 color;
in vec2 tc;
uniform sampler2D tex;
out vec4 pixel;
void main()
{
    pixel = texture(tex, tc);
}
