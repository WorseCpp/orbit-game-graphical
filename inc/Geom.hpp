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

#endif