#include "Sphere.h"

void Sphere::IntersectionPointToCoordinates(const Point3 &intersection_point, double &u, double &v)
{
    auto phi = std::atan2(-intersection_point.Z(), intersection_point.X()) + PI;
    auto theta = std::acos(-intersection_point.Y());

    u = phi / (2. * PI);
    v = theta / PI;
}

Sphere::Sphere(const Point3 &_start_center, double _radius, std::shared_ptr<Material> _material)
    : start_center(_start_center), radius(_radius), material(_material)
{
    auto radius_vector3 = Vector3(radius);
    bounding_box = AABB(_start_center - radius_vector3, _start_center + radius_vector3);
}

Sphere::Sphere(const Point3 &_start_center, const Point3 &_end_center, double _radius, std::shared_ptr<Material> _material)
    : start_center(_start_center), radius(_radius), material(_material)
{
    moving_direction = _end_center - _start_center;

    auto radius_vector3 = Vector3(radius);
    AABB start_box(_start_center - radius_vector3, _start_center + radius_vector3);
    AABB end_box(_end_center - radius_vector3, _end_center + radius_vector3);
    bounding_box = AABB(start_box, end_box);
}

bool Sphere::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    auto center = CurrentCenter(ray.Time());

    auto ray_origin = ray.Origin();
    auto ray_direction = ray.Direction();
    auto center_to_ray_origin = ray_origin - center;

    auto a = ray_direction.LengthSquared();
    auto half_b = Dot(ray_direction, center_to_ray_origin);
    auto c = center_to_ray_origin.LengthSquared() - radius * radius;

    auto delta = half_b * half_b - a * c;

    if (delta < 0.)
    {
        return false;
    }

    auto sqrt_delta = std::sqrt(delta);
    auto t = (-half_b - sqrt_delta) / a;
    if (!t_range.Surrounds(t))
    {
        t = (-half_b + sqrt_delta) / a;
        if (!t_range.Surrounds(t))
        {
            return false;
        }
    }

    hit_record.t = t;
    hit_record.intersection_point = ray.At(t);
    auto outward_normal = (hit_record.intersection_point - center) / radius;
    hit_record.SetFrontFaceAndNormal(ray, outward_normal);
    IntersectionPointToCoordinates(outward_normal, hit_record.u, hit_record.v);
    hit_record.material = material;

    return true;
}

Point3 Sphere::CurrentCenter(double time) const
{
    return start_center + time * moving_direction;
}
