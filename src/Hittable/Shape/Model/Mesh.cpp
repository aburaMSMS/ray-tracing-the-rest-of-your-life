#include "Mesh.h"
#include "Model.h"

Mesh::Mesh(size_t index1, size_t index2, size_t index3, Model &model)
    : face(model.vertices[index1].point, model.vertices[index2].point, model.vertices[index3].point, model.material)
{
    normals[0] = &(model.vertices[index1].normal);
    normals[1] = &(model.vertices[index2].normal);
    normals[2] = &(model.vertices[index3].normal);

    bounding_box = face.BoundingBox();
}

bool Mesh::IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const
{
    if (!face.IsHit(ray, t_range, hit_record))
    {
        return false;
    }

    auto alpha = hit_record.u;
    auto beta = hit_record.v;

    auto normal = UnitVector((1 - alpha - beta) * *normals[0] + alpha * *normals[1] + beta * *normals[2]);

    hit_record.SetFrontFaceAndNormal(ray, normal);

    return true;
}
