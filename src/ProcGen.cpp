#include "ProcGen.hpp"

// Constructor: initialize with a given capacity.
PlanetArray::PlanetArray(size_t inTheta, size_t inPhi)
{
    nTheta = (inTheta);
    nPhi = (inPhi);
    data = std::vector<std::vector<double>>(nTheta, std::vector<double>(nPhi, 1.0));
}

// Access element using angular coordinates in radians.
// theta should be in [0, pi] and phi in [0, 2*pi).
double& PlanetArray::operator()(double theta, double phi) {
    size_t row = angleToIndex(theta, 0.0, M_PI, nTheta);
    size_t col = angleToIndex(phi, 0.0, 2 * M_PI, nPhi);
    return data[row][col];
}

const double& PlanetArray::operator()(double theta, double phi) const {
    size_t row = angleToIndex(theta, 0.0, M_PI, nTheta);
    size_t col = angleToIndex(phi, 0.0, 2 * M_PI, nPhi);
    return data[row][col];
}

size_t PlanetArray::angleToIndex(double angle, double minAngle, double maxAngle, size_t divisions){
    double clamped = fmodf(angle - minAngle, maxAngle);
    // Normalize angle in [0, 1] then scale by divisions.
    double normalized = (clamped - minAngle) / (maxAngle - minAngle);
    // Ensure we stay within array bounds.
    size_t index = std::min(static_cast<size_t>(normalized * divisions), divisions - 1);
    return index;
}



// Specialization for SFloat3.
template <>
std::pair<std::vector<SFloat3>, std::vector<unsigned int>> PlanetArray::mesh<SFloat3>()
{
    std::vector<SFloat3> vertices;
    std::vector<unsigned int> indices;
    // Generate vertices.
    for (size_t i = 0; i < nTheta; ++i) {
        double theta = M_PI * static_cast<double>(i) / (nTheta - 1);
        for (size_t j = 0; j < nPhi; ++j) {
            double phi = 2.0 * M_PI * static_cast<double>(j) / nPhi;
            double r = data[i][j];

            // Convert from spherical to Cartesian coordinates.
            double x = r * sin(theta) * cos(phi);
            double y = r * cos(theta);
            double z = r * sin(theta) * sin(phi);

            vertices.push_back({static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)});
        }
    }

    // Generate indices for triangle mesh.
    for (size_t i = 0; i < nTheta - 1; ++i) {
        for (size_t j = 0; j < nPhi; ++j) {
            size_t next_j = (j + 1) % nPhi;
            unsigned int idx0 = i * nPhi + j;
            unsigned int idx1 = (i + 1) * nPhi + j;
            unsigned int idx2 = (i + 1) * nPhi + next_j;
            unsigned int idx3 = i * nPhi + next_j;

            // First triangle.
            indices.push_back(idx0);
            indices.push_back(idx1);
            indices.push_back(idx2);

            // Second triangle.
            indices.push_back(idx0);
            indices.push_back(idx2);
            indices.push_back(idx3);
        }
    }
    // TODO: Fill vertices and indices with the mesh data appropriate for SFloat3.
    // For example:
    // vertices.push_back({ x, y, z });
    // indices.push_back(0);

    

    return {vertices, indices};
}

// Specialization for SFloat3T2.
template <>
std::pair<std::vector<SFloat3T2>, std::vector<unsigned int>> PlanetArray::mesh<SFloat3T2>()
{
    std::vector<SFloat3T2> vertices;
    std::vector<unsigned int> indices;
    // Generate vertices.
    for (size_t i = 0; i < nTheta; ++i) {
        // theta from 0 to pi.
        double theta = M_PI * static_cast<double>(i) / (nTheta - 1);
        for (size_t j = 0; j < nPhi; ++j) {
            // phi from 0 to 2*pi.
            double phi = 2.0 * M_PI * static_cast<double>(j) / nPhi;
            double r = data[i][j];

            // Convert spherical to Cartesian coordinates.
            double x = r * sin(theta) * cos(phi);
            double y = r * cos(theta);
            double z = r * sin(theta) * sin(phi);

            // Compute texture coordinates.
            float u = static_cast<float>(j) / static_cast<float>(nPhi);
            float v = (nTheta > 1) ? static_cast<float>(i) / static_cast<float>(nTheta - 1) : 0.0f;

            vertices.push_back({
                static_cast<float>(x), static_cast<float>(y), static_cast<float>(z),
                u, v
            });
        }
    }

    // Generate indices for triangle mesh.
    for (size_t i = 0; i < nTheta - 1; ++i) {
        for (size_t j = 0; j < nPhi; ++j) {
            size_t next_j = (j + 1) % nPhi;
            unsigned int idx0 = i * nPhi + j;
            unsigned int idx1 = (i + 1) * nPhi + j;
            unsigned int idx2 = (i + 1) * nPhi + next_j;
            unsigned int idx3 = i * nPhi + next_j;
            
            // First triangle.
            indices.push_back(idx0);
            indices.push_back(idx1);
            indices.push_back(idx2);
            
            // Second triangle.
            indices.push_back(idx0);
            indices.push_back(idx2);
            indices.push_back(idx3);
        }
    }
    // TODO: Fill vertices and indices with the mesh data appropriate for SFloat3T2.
    // This could include texture coordinates in addition to position.
    
    return {vertices, indices};
}

// Specialization for P_N_C.
template <>
std::pair<std::vector<P_N_C>, std::vector<unsigned int>> PlanetArray::mesh<P_N_C>()
{
    std::vector<P_N_C> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices.
    // Loop over the angular grid.
    for (size_t i = 0; i < nTheta; ++i) {
        // theta goes from 0 to pi.
        double theta = M_PI * static_cast<double>(i) / (nTheta - 1);
        for (size_t j = 0; j < nPhi; ++j) {
            // phi goes from 0 to 2*pi.
            double phi = 2.0 * M_PI * static_cast<double>(j) / nPhi;
            // Use the stored planet data (e.g. height/scale factor).
            double r = data[i][j];

            // Compute 3D position from spherical coordinates.
            double x = r * sin(theta) * cos(phi);
            double y = r * cos(theta);
            double z = r * sin(theta) * sin(phi);

            // Compute normal: normalize the position vector.
            double len = sqrt(x * x + y * y + z * z);
            double nx = (len != 0.0) ? x / len : 0.0;
            double ny = (len != 0.0) ? y / len : 0.0;
            double nz = (len != 0.0) ? z / len : 0.0;

            // Derive a simple color from height.
            float fColor = static_cast<float>(r);

            // Assuming P_N_C is defined as {float x, y, z, nx, ny, nz, r, g, b},
            // push the vertex.
            vertices.push_back({
                glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)),
                glm::vec3(static_cast<float>(nx), static_cast<float>(ny), static_cast<float>(nz)),
                glm::vec3(fColor, fColor, fColor)
            });
        }
    }

    // Generate indices for a triangle mesh.
    // For each cell in the grid, form two triangles.
    for (size_t i = 0; i < nTheta - 1; ++i) {
        for (size_t j = 0; j < nPhi; ++j) {
            size_t next_j = (j + 1) % nPhi;
            unsigned int idx0 = i * nPhi + j;
            unsigned int idx1 = (i + 1) * nPhi + j;
            unsigned int idx2 = (i + 1) * nPhi + next_j;
            unsigned int idx3 = i * nPhi + next_j;
            
            // First triangle.
            indices.push_back(idx0);
            indices.push_back(idx1);
            indices.push_back(idx2);
            
            // Second triangle.
            indices.push_back(idx0);
            indices.push_back(idx2);
            indices.push_back(idx3);
        }
    }
    
    return {vertices, indices};
}