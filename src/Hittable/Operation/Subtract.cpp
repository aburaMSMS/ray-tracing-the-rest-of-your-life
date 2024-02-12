#include "Subtract.h"

bool Subtract::HitRecordComparer(const std::pair<HitRecord, HitRecord> &record1,
                                 const std::pair<HitRecord, HitRecord> &record2)
{
    return record1.first.t < record2.first.t;
}

Subtract::Subtract(std::shared_ptr<Hittable> _minuend, std::vector<std::shared_ptr<Hittable>> _subtracters)
    : minuend(_minuend), subtracters(_subtracters)
{
    bounding_box = minuend->BoundingBox();
}

bool Subtract::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    HitRecord minuend_hit_records[2];

    auto subtracters_size = subtracters.size();
    std::vector<std::pair<HitRecord, HitRecord>> subtracter_hit_records;
    subtracter_hit_records.resize(subtracters_size);

    double &minuend_t0 = minuend_hit_records[0].t, &minuend_t1 = minuend_hit_records[1].t;

    if (!minuend->IsHit(ray, t_range, minuend_hit_records[0]))
    {
        return false;
    }

    if (!minuend->IsHit(
            ray, Interval{minuend_hit_records[0].t + 0.0001, t_range.max + 0.0001}, minuend_hit_records[1]))
    {
        return false;
    }

    for (size_t i = 0; i < subtracters_size; i++)
    {
        if (!subtracters[i]->IsHit(ray, Interval::universe, subtracter_hit_records[i].first))
        {
            subtracter_hit_records[i].first.t = _INFINITY;
        }

        if (!subtracters[i]->IsHit(ray, Interval{subtracter_hit_records[i].first.t + 0.0001, _INFINITY}, subtracter_hit_records[i].second))
        {
            subtracter_hit_records[i].second.t = _INFINITY;
        }

        if (minuend_t0 > subtracter_hit_records[i].first.t &&
            minuend_t0 < subtracter_hit_records[i].second.t)
        {
            subtracter_hit_records[i].second.material = minuend_hit_records[0].material;
            minuend_hit_records[0] = subtracter_hit_records[i].second;
        }
    }

    std::sort(subtracter_hit_records.begin(), subtracter_hit_records.end(), HitRecordComparer);

    size_t subtracter_start_hit_record_index = 0, subtracter_end_hit_record_index = 0;

    for (size_t i = 0; i < subtracters_size - 1; i++)
    {
        if (subtracter_hit_records[i].second.t < 0.)
        {
            subtracter_start_hit_record_index = subtracter_end_hit_record_index = i + 1;
            continue;
        }

        if (subtracter_hit_records[subtracter_end_hit_record_index].second.t >
            subtracter_hit_records[i + 1].first.t)
        {
            if (subtracter_hit_records[subtracter_end_hit_record_index].second.t <
                subtracter_hit_records[i + 1].second.t)
            {
                subtracter_end_hit_record_index = i + 1;
            }
        }
        else
        {
            break;
        }
    }

    auto subtracter_t1 = subtracter_hit_records[subtracter_end_hit_record_index].second.t;
    if (subtracter_t1 < 0.)
    {
        return false;
    }

    auto subtracter_t0 =
        subtracter_hit_records[subtracter_start_hit_record_index].first.t > 0.
            ? subtracter_hit_records[subtracter_start_hit_record_index].first.t
            : 0.;

    if (subtracter_t0 <= minuend_t0)
    {
        if (subtracter_t1 < minuend_t0)
        {
            hit_record = minuend_hit_records[0];
            return true;
        }

        if (subtracter_t1 > t_range.max)
        {
            return false;
        }

        if (subtracter_t1 < minuend_t1)
        {
            hit_record = subtracter_hit_records[subtracter_end_hit_record_index].second;
            hit_record.material = minuend_hit_records[0].material;
            return true;
        }
    }
    else
    {
        hit_record = minuend_hit_records[0];
        return true;
    }

    return false;
}
