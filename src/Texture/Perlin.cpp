#include "Perlin.h"

void Perlin::Permute(int *perm)
{
    for (int i = points_num - 1; i > 0; i--)
    {
        auto j = RandomInt(0, i);

        std::swap(perm[i], perm[j]);
    }
}

int *Perlin::GeneratePerm()
{
    auto perm = new int[points_num];

    for (int i = 0; i < points_num; i++)
    {
        perm[i] = i;
    }

    Permute(perm);

    return perm;
}
double Perlin::TrilinearInterpolation(Vector3 volumes[2][2][2], double x, double y, double z)
{
    auto xx = x * x * (3. - 2. * x);
    auto yy = y * y * (3. - 2. * y);
    auto zz = z * z * (3. - 2. * z);

    auto value = 0.0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                Vector3 weight_vector3(x - i, y - j, z - k);
                value += (i * xx + (1 - i) * (1 - xx)) * (j * yy + (1 - j) * (1 - yy)) *
                         (k * zz + (1 - k) * (1 - zz)) * Dot(volumes[i][j][k], weight_vector3);
            }
        }
    }

    return value;
}

Perlin::Perlin()
{
    random_vector3s = new Vector3[points_num];

    for (int i = 0; i < points_num; i++)
    {
        random_vector3s[i] = RandomUnitVector3();
    }

    x_perm = GeneratePerm();
    y_perm = GeneratePerm();
    z_perm = GeneratePerm();
}

double Perlin::Noise(const Point3 &point) const
{
    auto x = point.X() - std::floor(point.X());
    auto y = point.Y() - std::floor(point.Y());
    auto z = point.Z() - std::floor(point.Z());

    auto i = static_cast<int>(std::floor(point.X()));
    auto j = static_cast<int>(std::floor(point.Y()));
    auto k = static_cast<int>(std::floor(point.Z()));

    Vector3 volumes[2][2][2]{};

    for (int delta_i = 0; delta_i < 2; delta_i++)
    {
        for (int delta_j = 0; delta_j < 2; delta_j++)
        {
            for (int delta_k = 0; delta_k < 2; delta_k++)
            {
                volumes[delta_i][delta_j][delta_k] = random_vector3s[x_perm[(i + delta_i) & 255] ^ y_perm[(j + delta_j) & 255] ^ z_perm[(k + delta_k) & 255]];
            }
        }
    }

    return TrilinearInterpolation(volumes, x, y, z);
}

double Perlin::Turbulence(Point3 point, int depth) const
{
    auto value = 0.;
    auto weight = 1.;

    for (int i = 0; i < depth; i++)
    {
        value += Noise(point) * weight;
        weight *= 0.5;
        point *= 2.;
    }

    return std::fabs(value);
}

Perlin::~Perlin()
{
    delete[] random_vector3s;
    delete[] x_perm;
    delete[] y_perm;
    delete[] z_perm;
}
