#include "Disk.h"

static const Vector3 normal{0., 1., 0.};

Disk::Disk(const Point3 &_center, double _radius, std::shared_ptr<Material> _material)
    : center(_center), radius(_radius), material(_material)
{
    auto half_length = Vector3{radius, 0., radius};
    bounding_box = AABB{center + half_length, center - half_length}.Pad();
}

bool Disk::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    auto origin = ray.Origin();
    auto direction = ray.Direction();

    auto t = (center.Y() - origin.Y()) / direction.Y();

    if (!t_range.Surrounds(t))
    {
        return false;
    }

    auto intersection_point = ray.At(t);

    if ((intersection_point - center).LengthSquared() - radius * radius > 0.0001)
    {
        return false;
    }

    /* TODO set uv*/

    hit_record.t = t;
    hit_record.intersection_point = intersection_point;
    hit_record.material = material;
    hit_record.SetFrontFaceAndNormal(ray, normal);

    return true;
}
