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

void P_N_C::setAttribPointer()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(P_N_C), (const void*)offsetof(P_N_C, pos));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(P_N_C), (const void*)offsetof(P_N_C, norm));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(P_N_C), (const void*)offsetof(P_N_C, color));
    glEnableVertexAttribArray(2);
}

P_N_C::P_N_C(const glm::vec3& p, const glm::vec3& n, const glm::vec3& c)
{
    pos = p;

    assert(glm::length(n) > 1e-6);

    norm = glm::normalize(n);

    color = c;
}

P_N_C::P_N_C()
{}