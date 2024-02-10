#pragma once

#include "Transformation.h"

/* Rotate */
class Rotate : public Transformation
{
public:
    Rotate(std::shared_ptr<Hittable> _object, double theta, int _axis_index)
        : Rotate(_object, theta, (_axis_index + 1) % 3, (_axis_index + 2) % 3) {}
    Rotate(std::shared_ptr<Hittable> _object, double theta, int _from_axis, int _to_axis);

    bool IsHit(const Ray &ray, Interval t_range, HitRecord &hit_record) const override;

private:
    void RotatePoint3(Point3 &point, bool rotate_direction = true) const;

private:
    double cos_theta;
    double sin_theta;
    int from_axis;
    int to_axis;
};
