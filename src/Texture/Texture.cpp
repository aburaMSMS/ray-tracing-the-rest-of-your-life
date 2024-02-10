#include "Texture.h"

Color SolidColor::Value(double u, double v, const Point3 &point) const
{
    return value;
}

Color CheckerTexture::Value(double u, double v, const Point3 &point) const
{
    auto x = static_cast<int>(std::floor(scale * point.X()));
    auto y = static_cast<int>(std::floor(scale * point.Y()));
    auto z = static_cast<int>(std::floor(scale * point.Z()));

    return (x + y + z) % 2 == 0 ? even->Value(u, v, point) : odd->Value(u, v, point);
}

Color ImageTexture::Value(double u, double v, const Point3 &point) const
{
    if (image.IsEmpty())
    {
        return {1., 0., 1.};
    }

    u = Interval(0., 1.).Clamp(u);
    v = 1. - Interval(0., 1.).Clamp(v);

    auto i = static_cast<int>(u * image.Width());
    auto j = static_cast<int>(v * image.Height());
    auto pixel = image.PixelData(i, j);

    auto scale = 1. / 255.;

    return {scale * pixel[0], scale * pixel[1], scale * pixel[2]};
}

Color NoiseTexture::Value(double u, double v, const Point3 &point) const
{
    return Color(1, 1, 1) * 0.5 * (1 + std::sin((scale * point)[axis] + 10 * perlin.Turbulence(scale * point / 8.)));
}
