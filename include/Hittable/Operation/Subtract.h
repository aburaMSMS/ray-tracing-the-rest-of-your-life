#pragma once

#include "Operation.h"

/* Subtract */
class Subtract : public Operation
{
public:
    Subtract(std::shared_ptr<Hittable> _minuend, std::shared_ptr<Hittable> _subtracter);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    std::shared_ptr<Hittable> minuend;
    std::shared_ptr<Hittable> subtracter;
};
