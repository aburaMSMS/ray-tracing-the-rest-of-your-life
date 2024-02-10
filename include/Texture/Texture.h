#pragma once

#include "Vector3.h"
#include "Color.h"
#include "memory"
#include "Image.h"
#include "Interval.h"
#include "Perlin.h"

class Texture
{
public:
    ~Texture() = default;

    virtual Color Value(double u, double v, const Point3 &point) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(const Color &_value) : value(_value) {}

    SolidColor(double red, double green, double blue) : value({red, green, blue}) {}

    Color Value(double u, double v, const Point3 &point) const override;

private:
    Color value;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(double _scale, std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
        : scale(_scale), even(_even), odd(_odd) {}

    CheckerTexture(double _scale, const Color &color1, const Color &color2)
        : scale(_scale),
          even(std::make_shared<SolidColor>(color1)),
          odd(std::make_shared<SolidColor>(color2)) {}

    Color Value(double u, double v, const Point3 &point) const override;

private:
    double scale;
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};

class ImageTexture : public Texture
{
public:
    ImageTexture(const char *image_filename) : image(image_filename) {}

    Color Value(double u, double v, const Point3 &point) const override;

private:
    Image image;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture(double _scale = 1., int _axis = 2) : scale(_scale), axis(_axis) {}

    Color Value(double u, double v, const Point3 &point) const override;

private:
    Perlin perlin;
    double scale;
    int axis;
};
