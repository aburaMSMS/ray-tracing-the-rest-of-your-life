#pragma once

#include "Vector3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Point3 &_origin, const Vector3 &_direction, double _time = 0.)
        : origin(_origin), direction(UnitVector(_direction)), time(_time){};

    Point3 Origin() const;
    Vector3 Direction() const;
    double Time() const;

    Point3 At(double t) const;

private:
    Point3 origin;
    Vector3 direction;
    double time = 0.;
};
