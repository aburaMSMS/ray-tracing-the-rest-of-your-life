#pragma once

#include "Shape.h"
#include "Quadrilateral.h"

/* Cuboid */
class Cuboid : public Shape
{
public:
    Cuboid(const Point3 &point1, const Point3 &point2,
           std::shared_ptr<Material> material);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const;

private:
    std::shared_ptr<HittableList> faces;
};
