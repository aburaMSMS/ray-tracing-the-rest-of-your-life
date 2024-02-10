#pragma once

#include "Shape.h"

/* Disk */
class Disk : public Shape
{
public:
    Disk(const Point3 &_center, double _radius, std::shared_ptr<Material> _material);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> material;
};
