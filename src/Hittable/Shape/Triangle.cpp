#include "Triangle.h"

Triangle::Triangle(const Point3 &_vertex1, const Point3 &_vertex2, const Point3 &_vertex3, std::shared_ptr<Material> _material)
    : vertex1(_vertex1), u(_vertex2 - _vertex1), v(_vertex3 - _vertex1), material(_material)
{
    auto n = Cross(u, v);
    normal = UnitVector(n);
    d = Dot(normal, vertex1);
    w = n / Dot(n, n);

    bounding_box = AABB(AABB(vertex1, _vertex2), AABB(vertex1, _vertex3)).Pad();
}

bool Triangle::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
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

    auto first_vertex_to_intersection = intersection - vertex1;
    auto alpha = Dot(w, Cross(first_vertex_to_intersection, v));
    auto beta = Dot(w, Cross(u, first_vertex_to_intersection));

    auto param_range = Interval(0., 1.);
    if (!(param_range.Contains(alpha) && param_range.Contains(beta) && alpha + beta <= 1.))
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
