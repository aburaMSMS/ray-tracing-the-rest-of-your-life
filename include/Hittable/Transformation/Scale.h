#pragma once

#include "Transformation.h"

/* Scale */
class Scale : public Transformation
{
public:
    Scale(std::shared_ptr<Hittable> _object, Vector3 _scale_rate);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    Vector3 scale_rate;
};
