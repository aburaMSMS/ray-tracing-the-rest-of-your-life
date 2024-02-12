#pragma once

#include "Operation.h"
#include "HittableList.h"

#include <vector>
#include <algorithm>

/* Subtract */
class Subtract : public Operation
{
public:
    static bool HitRecordComparer(const std::pair<HitRecord, HitRecord> &record1, const std::pair<HitRecord, HitRecord> &record2);

public:
    Subtract(std::shared_ptr<Hittable> _minuend, std::vector<std::shared_ptr<Hittable>> _subtracters);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    std::shared_ptr<Hittable> minuend;
    std::vector<std::shared_ptr<Hittable>> subtracters;
};
