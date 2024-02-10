#pragma once

#include "Transformation.h"

/* Translate */
class Translate : public Transformation
{
public:
    Translate(std::shared_ptr<Hittable> _object, const Vector3 &_offset);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    Vector3 offset;
};
