#pragma once

#include "Shape.h"
#include "Disk.h"

/* Cylinder */
class Cylinder : public Shape
{
public:
    Cylinder(const Point3 &_bottom_center, double _radius, double _height,
             std::shared_ptr<Material> _material);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const;

private:
    std::shared_ptr<HittableList> top_and_bottom;
    Point3 bottom_center;
    double radius;
    double height;
    std::shared_ptr<Material> material;
};
