#include "Hittable.h"

void HitRecord::SetFrontFaceAndNormal(const Ray &ray, const Vector3 &outward_normal)
{
    front_face = Dot(ray.Direction(), outward_normal) < 0.;
    normal = front_face ? outward_normal : -outward_normal;
}

AABB Hittable::BoundingBox() const
{
    return bounding_box;
}
