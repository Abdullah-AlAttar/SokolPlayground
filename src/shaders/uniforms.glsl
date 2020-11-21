@vs vs
in vec3 position;

uniform vs_params {
    mat4 transform;
};

void main() {
    gl_Position = transform * vec4(position, 1.0);
}
@end

@fs fs
uniform fs_params {
    vec4 ourColor;
};

out vec4 FragColor;

void main() {
    FragColor = ourColor;
}
@end

@program simple vs fs
