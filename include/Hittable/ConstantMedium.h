#pragma once

#include "Material.h"
#include "Hittable.h"

/* ConstantMedium */
class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> _boundary, double _density, std::shared_ptr<Texture> texture)
        : boundary(_boundary), density(_density), material(std::make_shared<Isotropic>(texture)) {}
    ConstantMedium(std::shared_ptr<Hittable> _boundary, double _density, const Color &color)
        : boundary(_boundary), density(_density), material(std::make_shared<Isotropic>(color)) {}

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    std::shared_ptr<Hittable> boundary;
    double density;
    std::shared_ptr<Material> material;
};
