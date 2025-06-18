#include "ProcGen.hpp"

#include "Perlin.hpp"

// Constructor: initialize with a given capacity.
PlanetArray::PlanetArray(size_t inTheta, size_t inPhi, double rad)
{
    nTheta = (inTheta);
    nPhi = (inPhi);
    nominal_rad = rad;
    data = std::vector<std::vector<double>>(nTheta, std::vector<double>(nPhi, rad));
}

void PlanetArray::fractal()
{
    double amplitude = 1.0;
    double frequency = 1. / 512.;
    double lacunarity = 2;
    double scale0 = .5;
    
    for (size_t n = 0; n < 10; n++) {
        // Set periods to nTheta*frequency and nPhi*frequency for seamless tiling
        auto p = PerlinNoise(mt_gen(), nTheta * frequency, nPhi * frequency);
    
        for (size_t i = 0; i < nTheta; i++) {
            
            float x = static_cast<float>(i) * frequency + 0.5f;

            for (size_t j = 0; j < nPhi; j++) {

                // Map i and j directly to [0, period)
                float y = static_cast<float>(j) * frequency + 0.5f;
    
                float noise_val = scale0 * p.noise(x, y);

                data[i][j] += noise_val * amplitude;
            }
        }
    
        frequency *= lacunarity;
        amplitude /= lacunarity;
    }
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

    PerlinNoise col_noise = PerlinNoise(mt_gen(), 32, 32);

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

            // Biome colors with natural variations
            glm::vec3 col;
            double height_above_nom = r - nominal_rad;

            double u = 32 * (i / (double) nTheta) + .5;
            double v = 32 * (j / (double) nPhi)   + .5;

            // Define biome thresholds (adjust based on your height distribution)
            const float DEEP_OCEAN = -0.f;
            const float SHALLOW_OCEAN = 0.2f;
            const float BEACH = 0.30f;
            const float GRASSLAND = 0.40f;
            const float FOREST = 0.55f;
            const float MOUNTAIN_BASE = 0.65f;
            const float SNOW_LINE = 0.75f;

            // Ocean biomes
            if (height_above_nom < DEEP_OCEAN) {
                col = glm::vec3(0.0f, 0.1f, 0.3f);  // Deep ocean
            } else if (height_above_nom < SHALLOW_OCEAN) {
                float t = (height_above_nom - DEEP_OCEAN) / (SHALLOW_OCEAN - DEEP_OCEAN);
                col = glm::mix(glm::vec3(0.0f, 0.1f, 0.3f), glm::vec3(0.2f, 0.5f, 0.9f), t);
            } 
            // Coastal biomes
            else if (height_above_nom < BEACH) {
                col = glm::vec3(0.96f, 0.96f, 0.7f);  // Sandy beach
            } 
            // Lowland biomes
            else if (height_above_nom < GRASSLAND) {
                // Add grassland variation using noise
                float noise = col_noise.noise(u, v) * 0.05f;
                col = glm::vec3(0.1f + noise, 0.7f + noise, 0.2f);
            } 
            // Mid-elevation biomes
            else if (height_above_nom < FOREST) {
                // Forest with natural color variation
                float noise = col_noise.noise(u, v) * 0.02f;
                col = glm::vec3(0.0f, 0.3f + noise, 0.05f);
            } 
            // Highland biomes
            else if (height_above_nom < MOUNTAIN_BASE) {
                // Rocky mountains with stratification
                float rock_variation = col_noise.noise(u, v) * 0.03f;
                col = glm::vec3(0.4f + rock_variation, 0.4f + rock_variation, 0.4f);
            } 
            // Alpine biomes
            else if (height_above_nom < SNOW_LINE) {
                float t = (height_above_nom - MOUNTAIN_BASE) / (SNOW_LINE - MOUNTAIN_BASE);
                glm::vec3 rock(0.5f, 0.5f, 0.5f);
                glm::vec3 snow(1.0f, 1.0f, 1.0f);
                col = glm::mix(rock, snow, t * 1.5f);  // Accelerated transition
            } 
            // Snow caps
            else {
                col = glm::vec3(1.0f, 1.0f, 1.0f);  // Pure snow
            }

            
            // Parameters
            float polar_band = 0.18f;   // Fraction of planet covered by polar ice at each pole (center of band)
            float polar_fade = 0.10f;   // Fraction for smooth transition/fade
            float noise_scale = 1.0f;   // Controls size of ice "fingers"
            float noise_strength = 0.01f; // How much the boundary "wiggles" (fraction of planet)

            // Normalized latitude: 0 at south pole, 1 at north pole
            float latitude = float(i) / float(nTheta - 1);
            float to_pole = std::min(latitude, 1.0f - latitude);

            // 1D Perlin noise based on phi (longitude)
            float phi_norm = float(j) / float(nPhi); // [0,1]
            float noise = col_noise.noise(phi_norm * 32. + .5, .5); // [-1,1] or [0,1] depending on your noise implementation

            // Offset the polar band with noise
            float polar_band_noisy = polar_band + noise * noise_strength;

            // Compute smooth polar mask (1.0 = full ice, 0.0 = no ice)
            float polar_mask = 0.0f;
            if (to_pole < polar_band_noisy) {
                float edge = polar_band_noisy - polar_fade;
                if (to_pole < edge) {
                    polar_mask = 1.0f;
                }
            }

            // Blend polar ice color with biome color
            glm::vec3 polar_ice_color(0.8f, 0.92f, 1.0f);
            col = glm::mix(col, polar_ice_color, polar_mask);

            // Assuming P_N_C is defined as {float x, y, z, nx, ny, nz, r, g, b},
            // push the vertex.
            vertices.push_back({
                glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)),
                glm::vec3(static_cast<float>(nx), static_cast<float>(ny), static_cast<float>(nz)),
                col
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