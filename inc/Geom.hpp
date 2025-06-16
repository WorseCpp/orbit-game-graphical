#include "common.hpp"

#ifndef GEOM_HPP
#define GEOM_HPP
#include "Verts.hpp"

std::pair<std::vector<SFloat3T2>, std::vector<unsigned int>> createSphere(float radius, float phi = 0.0f, float theta = 0.0f, int sectorCount = 36, int stackCount = 18) {
    std::vector<SFloat3T2> vertices;
    std::vector<unsigned int> indices;

    // Compute sphere vertices (SFloat3T2: x, y, z, u, v)
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * (PI / stackCount);  // from pi/2 to -pi/2
        float xy = cosf(stackAngle);
        float z = sinf(stackAngle);
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * (2 * PI / sectorCount);
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float u = static_cast<float>(j) / sectorCount;
            float v = static_cast<float>(i) / stackCount;

            vertices.push_back({ radius * x, radius * y, radius * z, u, v });
        }
    }

    const float cosPhi = cosf(phi);
    const float sinPhi = sinf(phi);
    const float cosTheta = cosf(theta);
    const float sinTheta = sinf(theta);

    for (auto& vertex : vertices) {
        float x = vertex.x;
        float y = vertex.y;
        float z = vertex.z;
        // First, rotate around Z axis by phi
        float x1 = x * cosPhi - y * sinPhi;
        float y1 = x * sinPhi + y * cosPhi;
        float z1 = z;
        // Next, rotate around X axis by theta
        vertex.x = x1;
        vertex.y = y1 * cosTheta - z1 * sinTheta;
        vertex.z = y1 * sinTheta + z1 * cosTheta;
    }

    // Generate sphere indices
    for (int i = 0; i < stackCount; ++i) {
        for (int j = 0; j < sectorCount; ++j) {
            int first = i * (sectorCount + 1) + j;
            int second = first + sectorCount + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
    

    return {vertices, indices};
}




std::pair<std::vector<P_N_C>, std::vector<unsigned int>> createSpherePNC(
    float radius, float phi = 0.0f, float theta = 0.0f, 
    int sectorCount = 36, int stackCount = 18)
{
    std::vector<P_N_C> vertices;
    std::vector<unsigned int> indices;

    // Compute sphere vertices (P_N_C: position, normal, color)
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * (PI / stackCount);  // from pi/2 to -pi/2
        float xy = cosf(stackAngle);
        float z = sinf(stackAngle);
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * (2 * PI / sectorCount);
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float modSector = fmodf(sectorAngle, 2.0f * PI);
            float modStack = fmodf(stackAngle, 2.0f * PI);
            srand(static_cast<unsigned int>(modSector * 1000 + modStack * 100000));

            float mult = 0.9f + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f;
            float posX = radius * x * mult;
            float posY = radius * y * mult;
            float posZ = radius * z * mult;

            // Compute and normalize normal (on a unit sphere, unscaled x,y,z are normalized, but we normalize anyway)
            float len = sqrtf(x * x + y * y + z * z);
            float nx = x / len;
            float ny = y / len;
            float nz = z / len;

            // Set color to green (r=0, g=1, b=0)
            vertices.push_back({ glm::vec3(posX, posY, posZ), glm::vec3(nx, ny, nz), glm::vec3(0.0f, 1.0f, 0.0f) });
        }
    }

    const float cosPhi = cosf(phi);
    const float sinPhi = sinf(phi);
    const float cosTheta = cosf(theta);
    const float sinTheta = sinf(theta);

    // Generate sphere indices
    for (int i = 0; i < stackCount; ++i) {
        for (int j = 0; j < sectorCount; ++j) {
            int first = i * (sectorCount + 1) + j;
            int second = first + sectorCount + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(first + 1);
            indices.push_back(second);
            indices.push_back(second + 1);
        }
    }
    
    return {vertices, indices};
}

#endif