#include "Cylinder.h"

Cylinder::Cylinder(const Point3 &_bottom_center, double _radius, double _height,
                   std::shared_ptr<Material> _material)
    : bottom_center(_bottom_center), radius(_radius), height(_height), material(_material)
{
    top_and_bottom = std::make_shared<HittableList>();

    top_and_bottom->Add(std::make_shared<Disk>(bottom_center, radius, material));

    auto top_center = bottom_center + Vector3{0., height, 0.};
    top_and_bottom->Add(std::make_shared<Disk>(top_center, radius, material));

    auto half_length = Vector3{radius, 0., radius};
    bounding_box = AABB{bottom_center - half_length, top_center + half_length}.Pad();
}

bool Cylinder::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    bool is_hit = false;

    if (top_and_bottom->IsHit(ray, t_range, hit_record))
    {
        is_hit = true;

        if (hit_record.t < t_range.max)
        {
            t_range.max = hit_record.t;
        }
    }

    auto center = bottom_center;
    auto origin = ray.Origin();
    auto direction = ray.Direction();

    center[1] = origin[1] = direction[1] = 0.;

    auto center_to_origin = origin - center;

    auto a = direction.LengthSquared();
    auto half_b = Dot(direction, center_to_origin);
    auto c = center_to_origin.LengthSquared() - radius * radius;

    auto delta = half_b * half_b - a * c;

    if (delta < 0.)
    {
        return is_hit;
    }

    auto sqrt_delta = std::sqrt(delta);
    auto t = (-half_b - sqrt_delta) / a;
    if (!t_range.Surrounds(t))
    {
        t = (-half_b + sqrt_delta) / a;
        if (!t_range.Surrounds(t))
        {
            return is_hit;
        }
    }

    auto intersection_point = ray.At(t);
    if (!Interval{bottom_center.Y(), bottom_center.Y() + height}.Contains(intersection_point.Y()))
    {
        return is_hit;
    }

    /* TODO set uv */

    hit_record.t = t;
    hit_record.intersection_point = intersection_point;
    hit_record.material = material;

    center[1] = intersection_point.Y();
    hit_record.SetFrontFaceAndNormal(ray, UnitVector(intersection_point - center));

    return true;
}
