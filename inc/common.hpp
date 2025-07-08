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

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::cout;
using std::endl;

constexpr bool DEBUG = false;

constexpr bool DEBUG_IBO = false || DEBUG;

constexpr bool DEBUG_VBO = false || DEBUG;


#endif

#define COMMON_HPP