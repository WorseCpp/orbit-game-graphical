#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aCol;
out vec3 col;
uniform mat4 MVP;
uniform float theta;

float clamp(float a, float b, float c)
{
    return min(b, max(a,c));
}

void main() {
    gl_Position = MVP * vec4(aPos, 1.0);
    col = aCol * clamp(0.0, 1.0, dot(aNorm, vec3(cos(theta), 0.0, sin(theta))));
}