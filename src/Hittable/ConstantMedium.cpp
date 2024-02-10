#include "ConstantMedium.h"

bool ConstantMedium::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    HitRecord hit_record1, hit_record2;

    if (!boundary->IsHit(ray, Interval::universe, hit_record1))
    {
        return false;
    }

    if (!boundary->IsHit(ray, Interval{hit_record1.t + 0.0001, _INFINITY}, hit_record2))
    {
        return false;
    }

    if (hit_record1.t < t_range.min)
    {
        hit_record1.t = t_range.min;
    }

    if (hit_record2.t > t_range.max)
    {
        hit_record2.t = t_range.max;
    }

    if (hit_record1.t < 0.)
    {
        hit_record1.t = 0.;
    }

    if (hit_record1.t > hit_record2.t)
    {
        return false;
    }

    auto direction_length = ray.Direction().Length();
    auto hittable_distance = (hit_record2.t - hit_record1.t) * direction_length;
    auto hit_distance = -1.0 / density * std::log(RandomDouble());

    if (hit_distance > hittable_distance)
    {
        return false;
    }

    hit_record.normal = Vector3{1., 0., 0.};
    hit_record.t = hit_distance / direction_length + hit_record1.t;
    hit_record.intersection_point = ray.At(hit_record.t);
    hit_record.material = material;

    return true;
}
