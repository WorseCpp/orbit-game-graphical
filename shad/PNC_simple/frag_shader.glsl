#version 330 core
in vec3 col;
out vec4 FragColor;

float clamp(float a, float b, float c)
{
    return min(b, max(a,c));
}



void main() {
    FragColor = vec4(clamp(0.0, 1.0, col.x), clamp(0.0, 1.0, col.y), clamp(0.0, 1.0, col.z), 1.0); // Ensure alpha is set to 1.0 for full opacity
}