#include "Color.h"

#include "Interval.h"

void WriteColor(std::ostream &out, Color pixel_color, double scale)
{
    static const Interval interval(0., 0.999);
    // Write the translated [0, 255] value of each color component.
    out << static_cast<int>(256. * interval.Clamp(LinearToGamma(pixel_color.X() * scale))) << ' '
        << static_cast<int>(256. * interval.Clamp(LinearToGamma(pixel_color.Y() * scale))) << ' '
        << static_cast<int>(256. * interval.Clamp(LinearToGamma(pixel_color.Z() * scale))) << '\n';
}
