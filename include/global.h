#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>

const double _INFINITY = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double Deg2Rad(const double deg)
{
    return deg * PI / 180.;
}

inline double RandomDouble(double min = 0., double max = 1.)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);

    return dis(gen);
}

inline int RandomInt(int min = 0, int max = 1)
{
    return static_cast<int>(RandomDouble(min, max + 1));
}
