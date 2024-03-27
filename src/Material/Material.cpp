#include "Material.h"

#include "Hittable.h"

Color Material::Emit(const HitRecord &hit_record, double u, double v, const Point3 &point) const
{
    return Color{0.};
}

double Material::ScatteringPdf(const Ray &incidente_ray, const HitRecord &hit_record, const Ray &scattered_ray) const
{
    return 0;
}

bool Lambertian::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                         Color &attenuation, Ray &scattered_ray) const
{
    ONB uvw;
    uvw.BuildFromW(hit_record.normal);
    auto scatter_direction = uvw.Local(RandomCosineDirection());

    attenuation = albedo->Value(hit_record.u, hit_record.v, hit_record.intersection_point);
    scattered_ray = Ray(hit_record.intersection_point, scatter_direction, incident_ray.Time());
    return true;
}

double Lambertian::ScatteringPdf(const Ray &incidente_ray, const HitRecord &hit_record, const Ray &scattered_ray) const
{
    auto cos_theta = Dot(hit_record.normal, UnitVector(scattered_ray.Direction()));

    return cos_theta < 0. ? 0. : cos_theta / PI;
}

bool OrenNayarDiffuse::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                               Color &attenuation, Ray &scattered_ray) const
{
    auto scatter_direction = RandomOnHemisphere(hit_record.normal);

    auto A = 0.18 * rou_d / (1. + rou_d);
    auto theta_i = Dot(incident_ray.Direction(), hit_record.normal) /
                   (incident_ray.Direction().Length() * hit_record.normal.Length());
    auto theta_o = Dot(scatter_direction, hit_record.normal) /
                   (scatter_direction.Length() * hit_record.normal.Length());

    auto alpha = (theta_i + theta_o) / 2.;
    auto beta = (theta_i - theta_o) / 2.;

    attenuation = 1. / PI * rou_d * (1. + A * std::fmax(0, std::cos(theta_i)) * std::fmax(0, std::cos(theta_o)) * std::sin(alpha) * std::tan(beta));
    scattered_ray = Ray(hit_record.intersection_point, scatter_direction, incident_ray.Time());
    return true;
}

bool Metal::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                    Color &attenuation, Ray &scattered_ray) const
{
    auto reflect_direction = Reflect(incident_ray.Direction(), hit_record.normal);
    reflect_direction += fuzz * RandomUnitVector3();
    attenuation = albedo;
    scattered_ray = Ray(hit_record.intersection_point, reflect_direction, incident_ray.Time());
    return Dot(hit_record.normal, scattered_ray.Direction()) > 0;
}

bool Dielectric::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                         Color &attenuation, Ray &scattered_ray) const
{
    auto incident_eta_over_transmitted_eta = hit_record.front_face ? 1. / refractive_index : refractive_index;

    auto incident_direction = incident_ray.Direction();

    auto cos_theta = std::fmin(Dot(-incident_direction, hit_record.normal), 1.);
    auto sin_theta = std::sqrt(1 - cos_theta * cos_theta);

    Vector3 scattered_direction;

    if (incident_eta_over_transmitted_eta * sin_theta > 1. ||
        FresnelInSchlick(cos_theta, incident_eta_over_transmitted_eta) > RandomDouble())
    {
        scattered_direction = Reflect(incident_direction, hit_record.normal);
    }
    else
    {
        scattered_direction = Refract(incident_direction, hit_record.normal, incident_eta_over_transmitted_eta);
    }

    scattered_ray = Ray(hit_record.intersection_point, scattered_direction, incident_ray.Time());
    return true;
}

double Dielectric::FresnelInSchlick(double cos_theta, double incident_eta_over_transmitted_eta)
{
    auto f0 = (1 - incident_eta_over_transmitted_eta) * (1 - incident_eta_over_transmitted_eta) /
              (1 + incident_eta_over_transmitted_eta) / (1 + incident_eta_over_transmitted_eta);
    return f0 + ((1 - f0) * (1 - cos_theta)) * ((1 - cos_theta) * (1 - cos_theta)) *
                    ((1 - cos_theta) * (1 - cos_theta)); // F��
}

Color DiffuseLight::Emit(const HitRecord &hit_record, double u, double v, const Point3 &point) const
{
    if (!hit_record.front_face)
    {
        return Color{0.};
    }
    return light_texture->Value(u, v, point);
}

bool DiffuseLight::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                           Color &attenuation, Ray &scattered_ray) const
{
    return false;
}

bool Isotropic::Scatter(const Ray &incident_ray, const HitRecord &hit_record,
                        Color &attenuation, Ray &scattered_ray) const
{
    scattered_ray = Ray{hit_record.intersection_point, RandomUnitVector3(), incident_ray.Time()};
    attenuation = albedo->Value(hit_record.u, hit_record.v, hit_record.intersection_point);
    return true;
}
