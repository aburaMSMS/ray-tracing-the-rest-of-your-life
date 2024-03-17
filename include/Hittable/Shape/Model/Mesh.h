#pragma once

#include "Triangle.h"

class Model;

/* Mesh */
class Mesh : public Shape
{
public:
    Mesh(size_t index1, size_t index2, size_t index3, Model &model);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    Triangle face;
    Vector3 *normals[3];

    friend class Model;
};
