uniform mat4 MVP;
uniform mat4 model_matrix;
uniform float radius;
in vec3 pos;

out vec3 world_pos;

void main() {
    vec4 p = vec4(pos * radius, 1.0);
    gl_Position = MVP * p;
    world_pos = (model_matrix * p).xyz;
}
