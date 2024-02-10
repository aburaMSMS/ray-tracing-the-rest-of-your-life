#include "Translate.h"

Translate::Translate(std::shared_ptr<Hittable> _object, const Vector3 &_offset)
    : Transformation(_object), offset(_offset)
{
    bounding_box = object->BoundingBox() + offset;
}

bool Translate::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    Ray offset_ray{ray.Origin() - offset, ray.Direction()};

    if (!object->IsHit(offset_ray, t_range, hit_record))
    {
        return false;
    }

    hit_record.intersection_point += offset;

    return true;
}
