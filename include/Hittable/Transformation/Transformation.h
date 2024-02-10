#pragma once

#include "Hittable.h"

/* Transformation */
class Transformation : public Hittable
{
protected:
    Transformation(std::shared_ptr<Hittable> _object) : object(_object) {}

protected:
    std::shared_ptr<Hittable> object;
};
