#include "Rotate.h"

Rotate::Rotate(std::shared_ptr<Hittable> _object, double theta, int _from_axis, int _to_axis)
    : Transformation(_object), from_axis(_from_axis), to_axis(_to_axis)
{
    theta = Deg2Rad(theta);
    cos_theta = std::cos(theta);
    sin_theta = std::sin(theta);

    Point3 min{_INFINITY};
    Point3 max{-_INFINITY};

    auto _bounding_box = object->BoundingBox();

    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            for (size_t k = 0; k < 2; k++)
            {
                auto point = _bounding_box.At(i, j, k);
                RotatePoint3(point);

                for (size_t l = 0; l < 3; l++)
                {
                    min[l] = std::fmin(min[l], point[l]);
                    max[l] = std::fmax(max[l], point[l]);
                }
            }
        }
    }

    bounding_box = AABB(min, max);
}

bool Rotate::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    auto origin = ray.Origin();
    auto direction = ray.Direction();

    RotatePoint3(origin, false);
    RotatePoint3(direction, false);

    Ray rotated_ray(origin, direction, ray.Time());

    if (!object->IsHit(rotated_ray, t_range, hit_record))
    {
        return false;
    }

    RotatePoint3(hit_record.intersection_point);
    RotatePoint3(hit_record.normal);

    return true;
}

void Rotate::RotatePoint3(Point3 &point, bool rotate_direction) const
{
    auto new_sin_theta = rotate_direction ? sin_theta : -sin_theta;
    auto from_axis_value = point[from_axis];
    auto to_axis_value = point[to_axis];

    point[from_axis] = cos_theta * from_axis_value - new_sin_theta * to_axis_value;
    point[to_axis] = new_sin_theta * from_axis_value + cos_theta * to_axis_value;
}
