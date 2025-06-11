#ifndef RENDERABLE_VERTICES_H
#define RENDERABLE_VERTICES_H

#include <vector>
#include <cstddef>

// Triangle class that inherits from Renderable
class SFloat3{
    public:
        // Override to allow reconfiguring the attribute pointer if needed
        void setAttribPointer();
    
        float x, y, z;
        SFloat3(float x, float y, float z);
        SFloat3();
};


#endif // RENDERABLE_VERTICES_H