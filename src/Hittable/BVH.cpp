#include "BVH.h"

bool BVH::BoxComparer(const std::shared_ptr<Hittable> box1, const std::shared_ptr<Hittable> box2,
                      int axis_index)
{
    return box1->BoundingBox().Axis(axis_index).min <
           box2->BoundingBox().Axis(axis_index).min;
}

bool BVH::BoxXComparer(const std::shared_ptr<Hittable> box1, const std::shared_ptr<Hittable> box2)
{
    return BoxComparer(box1, box2, 0);
}

bool BVH::BoxYComparer(const std::shared_ptr<Hittable> box1, const std::shared_ptr<Hittable> box2)
{
    return BoxComparer(box1, box2, 0);
}

bool BVH::BoxZComparer(const std::shared_ptr<Hittable> box1, const std::shared_ptr<Hittable> box2)
{
    return BoxComparer(box1, box2, 0);
}

BVH::BVH(const std::vector<std::shared_ptr<Hittable>> &source_objects, size_t start, size_t end)
{
    auto objects = source_objects;
    auto size = end - start;

    auto axis_index = RandomInt(0, 2);
    auto Comparer = axis_index == 0   ? BoxXComparer
                    : axis_index == 1 ? BoxYComparer
                                      : BoxZComparer;

    if (size == 1)
    {
        left = right = objects[start];
    }
    else if (size == 2)
    {
        if (Comparer(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin() + start, objects.begin() + end, Comparer);

        auto middle = start + size / 2;
        left = std::make_shared<BVH>(objects, start, middle);
        right = std::make_shared<BVH>(objects, middle, end);
    }

    bounding_box = AABB(left->BoundingBox(), right->BoundingBox());
}

bool BVH::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    if (!bounding_box.IsHit(ray, t_range))
    {
        return false;
    }

    bool hit_left = left->IsHit(ray, t_range, hit_record);
    bool hit_right = right->IsHit(ray, Interval(t_range.min, hit_left ? hit_record.t : t_range.max), hit_record);

    return hit_left || hit_right;
}
