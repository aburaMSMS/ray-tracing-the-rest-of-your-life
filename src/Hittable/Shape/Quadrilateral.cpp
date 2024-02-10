#include "Quadrilateral.h"

Quadrilateral::Quadrilateral(const Point3 &_first_vertex, const Vector3 &_u, const Vector3 &_v, std::shared_ptr<Material> _material)
    : first_vertex(_first_vertex), u(_u), v(_v), material(_material)
{
    auto n = Cross(u, v);
    normal = UnitVector(n);
    d = Dot(normal, first_vertex);
    w = n / Dot(n, n);

    bounding_box = AABB(first_vertex, first_vertex + u + v).Pad();
}

bool Quadrilateral::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    auto divisor = Dot(normal, ray.Direction());

    if (std::fabs(divisor) < 1e-8)
    {
        return false;
    }

    auto t = (d - Dot(normal, ray.Origin())) / divisor;
    if (!t_range.Surrounds(t))
    {
        return false;
    }

    auto intersection = ray.At(t);

    auto first_vertex_to_intersection = intersection - first_vertex;
    auto alpha = Dot(w, Cross(first_vertex_to_intersection, v));
    auto beta = Dot(w, Cross(u, first_vertex_to_intersection));

    auto param_range = Interval(0., 1.);
    if (!param_range.Contains(alpha) || !param_range.Contains(beta))
    {
        return false;
    }

    hit_record.u = alpha;
    hit_record.v = beta;
    hit_record.t = t;
    hit_record.intersection_point = intersection;
    hit_record.material = material;
    hit_record.SetFrontFaceAndNormal(ray, normal);

    return true;
}
