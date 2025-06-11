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