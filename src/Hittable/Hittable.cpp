#include "Hittable.h"

void HitRecord::operator=(const HitRecord &hit_record)
{
    this->u = hit_record.u;
    this->v = hit_record.v;
    this->t = hit_record.t;
    this->intersection_point = hit_record.intersection_point;
    this->normal = hit_record.normal;
    this->material = hit_record.material;
    this->front_face = hit_record.front_face;
}

void HitRecord::SetFrontFaceAndNormal(const Ray &ray, const Vector3 &outward_normal)
{
    front_face = Dot(ray.Direction(), outward_normal) < 0.;
    normal = front_face ? outward_normal : -outward_normal;
}

AABB Hittable::BoundingBox() const
{
    return bounding_box;
}
