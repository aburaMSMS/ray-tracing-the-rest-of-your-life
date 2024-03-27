#include "ONB.h"

Vector3 ONB::operator[](int i) const
{
    return axis[i];
}
Vector3 &ONB::operator[](int i)
{
    return axis[i];
}

Vector3 ONB::U() const
{
    return axis[0];
}
Vector3 ONB::V() const
{
    return axis[1];
}
Vector3 ONB::W() const
{
    return axis[2];
}

Vector3 ONB::Local(double a, double b, double c) const
{
    return a * axis[0] + b * axis[1] + c * axis[2];
}

Vector3 ONB::Local(const Vector3 &v) const
{
    return v.X() * axis[0] + v.Y() * axis[1] + v.Z() * axis[1];
}

void ONB::BuildFromW(const Vector3 &w)
{
    Vector3 unit_w = UnitVector(w);
    Vector3 a = (std::fabs(unit_w.X()) > 0.9) ? Vector3{0., 1., 0.} : Vector3{1., 0., 0.};
    Vector3 v = UnitVector(Cross(unit_w, a));
    Vector3 u = Cross(unit_w, v);
    axis[0] = u;
    axis[1] = v;
    axis[2] = unit_w;
}
