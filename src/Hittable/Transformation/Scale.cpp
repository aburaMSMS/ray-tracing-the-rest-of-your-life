#include "Scale.h"

Scale::Scale(std::shared_ptr<Hittable> _object, Vector3 _scale_rate)
    : Transformation(_object), scale_rate(_scale_rate)
{
    bounding_box = object->BoundingBox() * scale_rate;
}

bool Scale::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    // TODO
    // maybe need normal map
    auto reverse_scale_rate = 1. / scale_rate;

    auto origin = ray.Origin() * reverse_scale_rate;
    auto direction = ray.Direction() * reverse_scale_rate;

    if (!object->IsHit(Ray{origin, direction, ray.Time()}, t_range, hit_record))
    {
        return false;
    }

    hit_record.intersection_point = hit_record.intersection_point * scale_rate;
    hit_record.normal = hit_record.normal * reverse_scale_rate;

    return true;
}
