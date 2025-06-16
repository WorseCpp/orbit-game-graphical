#version 330 core
in vec3 col;
out vec4 FragColor;
void main() {
    FragColor = vec4(col.x, col.y, col.z, 1.0); // Ensure alpha is set to 1.0 for full opacity
}