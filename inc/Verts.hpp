#ifndef RENDERABLE_VERTICES_H
#define RENDERABLE_VERTICES_H

#include <vector>
#include <cstddef>

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



#endif // RENDERABLE_VERTICES_H