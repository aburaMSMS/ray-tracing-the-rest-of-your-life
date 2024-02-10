#pragma once

#include "global.h"

class Interval
{
public:
    static const Interval empty, universe;

public:
    double min;
    double max;

    Interval() : min(_INFINITY), max(-_INFINITY) {}
    Interval(double _min, double _max) : min(_min), max(_max) {}
    Interval(const Interval &i1, const Interval &i2)
        : min(std::fmin(i1.min, i2.min)), max(std::fmax(i1.max, i2.max)) {}

    bool Contains(double value) const;

    bool Surrounds(double value) const;

    double Clamp(double x) const;

    double Size() const;

    Interval Expand(double delta) const;
};

inline Interval operator+(const Interval &interval, double offset)
{
    return Interval{interval.min + offset, interval.max + offset};
}

inline Interval operator+(double offset, const Interval &interval)
{
    return interval + offset;
}

inline Interval operator*(const Interval &interval, double scale)
{
    return Interval{interval.min * scale, interval.max * scale};
}

inline Interval operator*(double scale, const Interval &interval)
{
    return interval * scale;
}
