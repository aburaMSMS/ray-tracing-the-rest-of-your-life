#include "Cuboid.h"

Cuboid::Cuboid(const Point3 &point1, const Point3 &point2,
               std::shared_ptr<Material> material)
{

    Point3 min_point{
        std::fmin(point1.X(), point2.X()), std::fmin(point1.Y(), point2.Y()), std::fmin(point1.Z(), point2.Z())};
    Point3 max_point{
        std::fmax(point1.X(), point2.X()), std::fmax(point1.Y(), point2.Y()), std::fmax(point1.Z(), point2.Z())};

    Vector3 x_direction{max_point.X() - min_point.X(), 0., 0.};
    Vector3 y_direction{0., max_point.Y() - min_point.Y(), 0.};
    Vector3 z_direction{0., 0., max_point.Z() - min_point.Z()};

    faces = std::make_shared<HittableList>();

    faces->Add(std::make_shared<Quadrilateral>(min_point, x_direction, y_direction, material));
    faces->Add(std::make_shared<Quadrilateral>(min_point, x_direction, z_direction, material));
    faces->Add(std::make_shared<Quadrilateral>(min_point, y_direction, z_direction, material));
    faces->Add(std::make_shared<Quadrilateral>(max_point, -x_direction, -y_direction, material));
    faces->Add(std::make_shared<Quadrilateral>(max_point, -x_direction, -z_direction, material));
    faces->Add(std::make_shared<Quadrilateral>(max_point, -y_direction, -z_direction, material));

    bounding_box = AABB(point1, point2).Pad();
}

bool Cuboid::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    return faces->IsHit(ray, t_range, hit_record);
}
