#include "AABB.h"

AABB::AABB(const Point3 &p1, const Point3 &p2)
{
    x = Interval(std::fmin(p1[0], p2[0]), std::fmax(p1[0], p2[0]));
    y = Interval(std::fmin(p1[1], p2[1]), std::fmax(p1[1], p2[1]));
    z = Interval(std::fmin(p1[2], p2[2]), std::fmax(p1[2], p2[2]));
}

AABB::AABB(const AABB &box1, const AABB &box2)
{
    x = Interval(box1.x, box2.x);
    y = Interval(box1.y, box2.y);
    z = Interval(box1.z, box2.z);
}

const Interval &AABB::Axis(int axis_index) const
{
    if (axis_index == 0)
    {
        return x;
    }

    if (axis_index == 1)
    {
        return y;
    }

    return z;
}

bool AABB::IsHit(const Ray &ray, Interval t_range) const
{
    for (int i = 0; i < 3; i++)
    {
        auto axis = Axis(i);
        auto origin = ray.Origin()[i];
        auto divisor = 1. / ray.Direction()[i];

        auto t0 = (axis.min - origin) * divisor;
        auto t1 = (axis.max - origin) * divisor;

        if (t1 < t0)
        {
            std::swap(t0, t1);
        }

        if (t0 > t_range.min)
        {
            t_range.min = t0;
        }
        if (t1 < t_range.max)
        {
            t_range.max = t1;
        }

        if (t_range.min >= t_range.max)
        {
            return false;
        }
    }

    return true;
}

AABB AABB::Pad() const
{
    double delta = 0.0001;
    Interval new_x = (x.Size() >= delta) ? x : x.Expand(delta);
    Interval new_y = (y.Size() >= delta) ? y : y.Expand(delta);
    Interval new_z = (z.Size() >= delta) ? z : z.Expand(delta);

    return AABB(new_x, new_y, new_z);
}
