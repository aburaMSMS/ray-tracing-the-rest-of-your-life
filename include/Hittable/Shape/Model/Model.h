#pragma once

#include "Vector3.h"
#include "Mesh.h"
#include "BVH.h"

#include <string>
#include <fstream>
#include <sstream>

struct Vertex
{
    Point3 point;
    Vector3 normal{0.};

    Vertex(double x, double y, double z) : point(x, y, z) {}
};

/* Model */
class Model : public Shape
{
public:
    Model(const std::string &model_filename, std::shared_ptr<Material> _material);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    void LoadModel(const std::string &model_filename);

private:
    std::vector<Vertex> vertices;
    std::shared_ptr<Hittable> objects;
    std::shared_ptr<Material> material;

    friend class Mesh;
};
