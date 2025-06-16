#ifndef RENDERABLE_VERTICES_H
#define RENDERABLE_VERTICES_H

#include <vector>
#include <cstddef>

#include "glm/glm.hpp"

// Triangle class
class SFloat3{
    public:
        // Override to allow reconfiguring the attribute pointer if needed
        void setAttribPointer();
    
        float x, y, z;
        SFloat3(float x, float y, float z);
        SFloat3();
};

class SFloat3T2{
    public:
        // Override to allow reconfiguring the attribute pointer if needed
        void setAttribPointer();
    
        float x, y, z;
        float u, v;
        SFloat3T2(float x, float y, float z, float u, float v);
        SFloat3T2();
};

class P_N_C {
public:
    void setAttribPointer();

    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec3 color;

    P_N_C(const glm::vec3& p, const glm::vec3& n, const glm::vec3& c);
    P_N_C();
};


#endif // RENDERABLE_VERTICES_H