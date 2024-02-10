#pragma once

#include "Interval.h"
#include "Vector3.h"
#include "Ray.h"

class AABB
{
public:
    AABB() {}
    AABB(const Interval &_x, const Interval &_y, const Interval &_z) : x(_x), y(_y), z(_z) {}
    AABB(const Interval interval[3]) : AABB(interval[0], interval[1], interval[2]) {}
    AABB(const Point3 &p1, const Point3 &p2);
    AABB(const AABB &box1, const AABB &box2);

    const Interval &Axis(int axis_index) const;

    bool IsHit(const Ray &ray, Interval t_range) const;

    inline Point3 At(size_t i, size_t j, size_t k) const
    {
        return Point3{x.min + i * x.Size(), y.min + j * y.Size(), z.min + k * z.Size()};
    }

    AABB Pad() const;

public:
    Interval x, y, z;
};

inline AABB operator+(const AABB &bounding_box, const Vector3 &offset)
{
    return AABB{bounding_box.x + offset.X(), bounding_box.y + offset.Y(), bounding_box.z + offset.Z()};
}

inline AABB operator+(const Vector3 &offset, const AABB &bounding_box)
{
    return bounding_box + offset;
}

inline AABB operator*(const AABB &bounding_box, double scale)
{
    return AABB{bounding_box.x * scale, bounding_box.y * scale, bounding_box.z * scale};
}

inline AABB operator*(double scale, const AABB &bounding_box)
{
    return bounding_box * scale;
}

inline AABB operator*(const AABB &bounding_box, Vector3 scale)
{
    return AABB{bounding_box.x * scale.X(), bounding_box.y * scale.Y(), bounding_box.z * scale.Z()};
}

inline AABB operator*(Vector3 scale, const AABB &bounding_box)
{
    return bounding_box * scale;
}
