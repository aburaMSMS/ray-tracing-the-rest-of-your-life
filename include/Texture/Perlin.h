#pragma once

#include "global.h"
#include "Vector3.h"

class Perlin
{
private:
    static void Permute(int *perm);

    static int *GeneratePerm();

    static double TrilinearInterpolation(Vector3 volume[2][2][2], double x, double y, double z);

private:
    static const int points_num = 256;

public:
    Perlin();

    double Noise(const Point3 &point) const;

    double Turbulence(Point3 point, int depth = 7) const;

    ~Perlin();

private:
    Vector3 *random_vector3s;
    int *x_perm;
    int *y_perm;
    int *z_perm;
};
