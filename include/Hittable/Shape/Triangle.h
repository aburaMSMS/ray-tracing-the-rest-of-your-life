#pragma once

#include "Shape.h"
#include "Vector3.h"
#include "Material.h"
#include "AABB.h"
#include "HittableList.h"

class Triangle : public Shape
{
public:
    Triangle(const Point3 &_vertex1, const Point3 &_vertex2, const Point3 &_vertex3, std::shared_ptr<Material> _material);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const;

private:
    const Point3 &vertex1;
    Vector3 u, v;
    std::shared_ptr<Material> material;
    Vector3 normal;
    double d = 0.;
    Vector3 w;
};
