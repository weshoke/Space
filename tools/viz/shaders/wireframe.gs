#version 150

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform vec2 screen_size;

out vec4 color;
noperspective out vec3 pixDistance;

void main() {
    // taken from 'Single-Pass Wireframe Rendering'
    vec2 p0 = screen_size * gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
    vec2 p1 = screen_size * gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;
    vec2 p2 = screen_size * gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w;

    vec2 v0 = p2 - p1;
    vec2 v1 = p2 - p0;
    vec2 v2 = p1 - p0;
    float fArea = abs( v1.x * v2.y - v1.y * v2.x );

    pixDistance = vec3( fArea / length( v0 ), 0, 0 );
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    pixDistance = vec3( 0, fArea / length( v1 ), 0 );
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    pixDistance = vec3( 0, 0, fArea / length( v2 ) );
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
