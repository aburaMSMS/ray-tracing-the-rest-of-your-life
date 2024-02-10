#include "HittableList.h"

HittableList::HittableList(const std::shared_ptr<Hittable> object)
{
    Add(object);
}

void HittableList::Clear()
{
    objects.clear();
}

void HittableList::Add(const std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
    bounding_box = AABB(bounding_box, object->BoundingBox());
}

size_t HittableList::Size() const
{
    return objects.size();
}

bool HittableList::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    HitRecord temp_record;
    auto closest_t = t_range.max;
    bool is_hit_anything = false;

    for (const auto &object : objects)
    {
        if (object->IsHit(ray, Interval(t_range.min, closest_t), temp_record))
        {
            is_hit_anything = true;
            closest_t = temp_record.t;
            hit_record = temp_record;
        }
    }

    return is_hit_anything;
}
