#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

#include "common.hpp"

#include "Verts.hpp"

class PlanetArray {
public:
    // Constructs a planet grid with nTheta rows (from 0 to pi) and nPhi columns (from 0 to 2pi)
    PlanetArray(size_t nTheta, size_t nPhi);

    // Access element using angular coordinates in radians.
    // theta should be in [0, pi] and phi in [0, 2*pi).
    double& operator()(double theta, double phi);

    const double& operator()(double theta, double phi) const;

    template <HasAttribPointer T>
    std::pair<std::vector<T>, std::vector<unsigned int>> mesh();

private:
    size_t nTheta, nPhi;
    std::vector<std::vector<double>> data;

    // Converts an angle value to an index in the range [0, divisions-1].
    static size_t angleToIndex(double angle, double minAngle, double maxAngle, size_t divisions);


};
