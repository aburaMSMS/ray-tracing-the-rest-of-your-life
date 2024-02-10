#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "Hittable.h"
#include "HittableList.h"
#include "AABB.h"

class BVH : public Hittable
{
public:
    static bool BoxComparer(const std::shared_ptr<Hittable> box1,
                            const std::shared_ptr<Hittable> box2, int axis_index);
    static bool BoxXComparer(const std::shared_ptr<Hittable> box1,
                             const std::shared_ptr<Hittable> box2);
    static bool BoxYComparer(const std::shared_ptr<Hittable> box1,
                             const std::shared_ptr<Hittable> box2);
    static bool BoxZComparer(const std::shared_ptr<Hittable> box1,
                             const std::shared_ptr<Hittable> box2);

public:
    BVH(const HittableList &list) : BVH(list.objects, 0, list.Size()) {}
    BVH(const std::vector<std::shared_ptr<Hittable>> &source_objects, size_t start, size_t end);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
};
