#include "Subtract.h"

Subtract::Subtract(std::shared_ptr<Hittable> _minuend, std::shared_ptr<Hittable> _subtracter)
    : minuend(_minuend), subtracter(_subtracter)
{
    bounding_box = minuend->BoundingBox();
}

bool Subtract::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    HitRecord minuend_hit_record[2], subtracter_hit_record[2];

    double &minuend_t0 = minuend_hit_record[0].t, &minuend_t1 = minuend_hit_record[1].t,
           &subtracter_t0 = subtracter_hit_record[0].t, &subtracter_t1 = subtracter_hit_record[1].t;

    if (!minuend->IsHit(ray, t_range, minuend_hit_record[0]))
    {
        return false;
    }

    if (!minuend->IsHit(
            ray, Interval{minuend_hit_record[0].t + 0.0001, t_range.max}, minuend_hit_record[1]))
    {
        minuend_hit_record[1].t = minuend_hit_record[0].t + 0.0001;
    }

    if (!subtracter->IsHit(ray, t_range, subtracter_hit_record[0]))
    {
        hit_record = minuend_hit_record[0];
        return true;
    }

    if (!subtracter->IsHit(
            ray, Interval{subtracter_hit_record[0].t + 0.0001, _INFINITY}, subtracter_hit_record[1]))
    {
        if (minuend_t0 > subtracter_t0)
        {
            hit_record = subtracter_hit_record[0];
            hit_record.material = minuend_hit_record[0].material;
            return true;
        }

        return false;
    }

    if (subtracter_t0 < minuend_t0)
    {
        if (subtracter_t1 < minuend_t0)
        {
            hit_record = minuend_hit_record[0];
            return true;
        }

        if (subtracter_t1 < minuend_t1)
        {
            hit_record = subtracter_hit_record[1];
            hit_record.material = minuend_hit_record[0].material;
            return true;
        }
    }
    else
    {
        hit_record = minuend_hit_record[0];
        return true;
    }

    return false;
}
