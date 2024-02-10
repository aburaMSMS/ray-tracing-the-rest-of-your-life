#pragma once

#include "global.h"

#include <cmath>
#include <iostream>

class Vector3
{
public:
    static Vector3 Random(double min = 0., double max = 1.)
    {
        return Vector3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
    }

public:
    double e[3];

    Vector3() : e{0., 0., 0.} {}
    Vector3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    Vector3(double e0) : e{e0, e0, e0} {}
    Vector3(const Vector3 &v) : e{v[0], v[1], v[2]} {}

    double X() const { return e[0]; }
    double Y() const { return e[1]; }
    double Z() const { return e[2]; }

    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    double operator[](size_t i) const { return e[i]; }
    double &operator[](size_t i) { return e[i]; }

    Vector3 &operator+=(const Vector3 &v)
    {
        e[0] += v[0];
        e[1] += v[1];
        e[2] += v[2];
        return *this;
    }

    Vector3 &operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3 &operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double Length() const
    {
        return std::sqrt(LengthSquared());
    }

    double LengthSquared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool IsNearZero() const
    {
        const static auto epsilon = 1e-8;
        return std::fabs(e[0]) < epsilon && std::fabs(e[1]) < epsilon && std::fabs(e[2]) < epsilon;
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using Point3 = Vector3;

// Vector Utility Functions

inline std::ostream &operator<<(std::ostream &out, const Vector3 &v)
{
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline Vector3 operator*(double t, const Vector3 &v)
{
    return Vector3(t * v[0], t * v[1], t * v[2]);
}

inline Vector3 operator*(const Vector3 &v, double t)
{
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t)
{
    return (1 / t) * v;
}

inline Vector3 operator/(double t, Vector3 v)
{
    return Vector3{1. / v[0], 1. / v[1], 1. / v[2]};
}

inline double Dot(const Vector3 &vector1, const Vector3 &vector2)
{
    return vector1[0] * vector2[0] + vector1[1] * vector2[1] + vector1[2] * vector2[2];
}

inline Vector3 Cross(const Vector3 &u, const Vector3 &v)
{
    return Vector3(u[1] * v[2] - u[2] * v[1],
                   u[2] * v[0] - u[0] * v[2],
                   u[0] * v[1] - u[1] * v[0]);
}

inline Vector3 UnitVector(Vector3 v)
{
    return v / v.Length();
}

inline Vector3 RandomUnitVector3()
{
    while (true)
    {
        auto outward_direction = Vector3::Random(-1., 1.);
        if (outward_direction.LengthSquared() < 1.)
        {
            return UnitVector(outward_direction);
        }
    }
}

inline Vector3 RandomOnHemisphere(const Vector3 &normal)
{
    auto outward_direction = RandomUnitVector3();

    return Dot(normal, outward_direction) > 0. ? outward_direction : -outward_direction;
}

inline Vector3 Refract(const Vector3 &incident_direction, const Vector3 &normal,
                       double incident_eta_over_transmitted_eta)
{
    auto cos_theta = std::fmin(Dot(-incident_direction, normal), 1.);
    auto perpendicular_outward_r = incident_eta_over_transmitted_eta * (incident_direction +
                                                                        cos_theta * normal);
    auto parallel_outward_r = -std::sqrt(1 - perpendicular_outward_r.LengthSquared()) * normal;

    return perpendicular_outward_r + parallel_outward_r; // R'
}

inline Vector3 RandomPoint3InUnitDisk()
{
    while (true)
    {
        auto point = Point3(RandomDouble(-1., 1.), RandomDouble(-1., 1.), 0.);
        if (point.LengthSquared() < 1.)
        {
            return point;
        }
    }
}

inline Vector3 Reflect(const Vector3 &incident_direction, const Vector3 &normal)
{
    return incident_direction - 2 * Dot(incident_direction, normal) * normal;
}
