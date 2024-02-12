#pragma once

#include "Hittable.h"
#include "Interval.h"
#include "AABB.h"

#include <vector>
#include <memory>

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(const std::shared_ptr<Hittable> object);
    HittableList(const std::vector<std::shared_ptr<Hittable>> &_objects);

    void Clear();

    void Add(const std::shared_ptr<Hittable> object);

    size_t Size() const;

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

public:
    std::vector<std::shared_ptr<Hittable>> objects;
};
