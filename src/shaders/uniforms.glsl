@vs vs_uni
in vec3 position;

uniform vs_params {
    mat4 transform;
};

void main() {
    gl_Position = transform * vec4(position, 1.0);
}
@end

@fs fs_uni
uniform fs_params {
    vec4 ourColor;
};

out vec4 FragColor;

void main() {
    FragColor = ourColor;
}
@end

@program simple_uniform vs_uni fs_uni
