#pragma once

#include "Vector3.h"
#include <iostream>

using Color = Vector3;

inline double LinearToGamma(double linear_component)
{
    return sqrt(linear_component);
}

void WriteColor(std::ostream &out, Color pixel_color, double scale);
