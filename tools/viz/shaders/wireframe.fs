#version 150

uniform vec4 color;

noperspective in vec3 pixDistance;
out vec4 fragColor;

void main() {
    // Determine frag distance to closest edge
    float nearest = min(min(pixDistance[0], pixDistance[1]), pixDistance[2]);
    float edge_intensity = exp2(-1.0 * nearest * nearest);

    // Blend between edge color and face color
    vec3 face_color = color.rgb;
    vec3 edge_color = face_color * 0.8;

    fragColor.rgb = mix(face_color, edge_color, edge_intensity);
    fragColor.a = 1.;
}
