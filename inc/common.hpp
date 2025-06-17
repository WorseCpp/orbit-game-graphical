#pragma once

#ifndef COMMON_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <random>
#include <utility>
#include <algorithm>
#include <numeric>
#include <cmath>

const float PI = 3.14159265f;

static std::random_device rand_device;
static std::mt19937 mt_gen(0);

#endif

#define COMMON_HPP