#include "Verts.hpp"

#include "common.hpp"

void SFloat3::setAttribPointer() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}


SFloat3::SFloat3(float x, float y, float z)
    : x(x), y(y), z(z) {
}

SFloat3::SFloat3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

void SFloat3T2::setAttribPointer() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
SFloat3T2::SFloat3T2(float x, float y, float z, float u, float v)
    : x(x), y(y), z(z), u(u), v(v) {
}
SFloat3T2::SFloat3T2()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    u = 0.0f;
    v = 0.0f;
}