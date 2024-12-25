#ifndef UTILITY_H
#define UTILITY_H

// Libraries
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <memory>

// Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Functions
inline double deg_to_rad(double degrees) {
	return degrees * pi/180;
}

inline double random_double(double min, double max) {
	std::random_device dev; // Random number wizardy??
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> dist(min,max); // distribution in range [1, 6]

    return dist(rng);
}

// Includes
#include "interval.h"
#include "vector.h"
#include "ray.h"
#include "image.h"
#include "hittable.h"
#include "scene.h"
#include "sphere.h"
#include "camera.h"
#include "progress.h"

#endif
