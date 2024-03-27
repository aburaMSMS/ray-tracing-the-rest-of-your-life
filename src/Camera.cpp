#include "Camera.h"

void Camera::Render(const HittableList &world)
{
    Initialize();

    // Render
    auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << "P3\n"
              << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            Color pixel_color{0.};

            for (int sample_j = 0; sample_j < sqrt_samples_per_pixel; sample_j++)
            {
                for (int sample_i = 0; sample_i < sqrt_samples_per_pixel; sample_i++)
                {
                    auto sample_ray = GetRay(i, j, sample_i, sample_j);
                    pixel_color += RayColor(sample_ray, max_depth, world);
                }
            }

            WriteColor(std::cout, pixel_color, 1. / samples_per_pixel);
        }
    }

    std::clog << "\rDone.                 \n";

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);

    std::clog << "Rendering Time: " << duration.count() << " seconds" << std::endl;
}

void Camera::Initialize()
{
    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    sqrt_samples_per_pixel = static_cast<int>(std::sqrt(samples_per_pixel));
    recip_sqrt_samples_per_pixel = 1. / std::sqrt(samples_per_pixel);

    camera_center = look_from;

    // Determine viewport dimensions.
    auto vertical_fov_rad = Deg2Rad(vertical_fov);
    auto viewport_height = 2. * focus_distance * std::tan(vertical_fov_rad / 2.);
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    w = UnitVector(look_from - look_at);
    u = UnitVector(Cross(view_up, w));
    v = Cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = viewport_width * u;
    auto viewport_v = viewport_height * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - focus_distance * w - viewport_u / 2. - viewport_v / 2.;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    auto defocus_radius = focus_distance * std::tan(Deg2Rad(defocus_angle) / 2.);
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

Ray Camera::GetRay(int i, int j, int sample_i, int sample_j) const
{
    auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
    auto pixel_sample = pixel_center + PixelSampleSquare(sample_i, sample_j);

    Point3 ray_origin = defocus_angle <= 0. ? camera_center : DefocusDiskSample();
    auto time = RandomDouble();

    return Ray(ray_origin, pixel_sample - ray_origin, time);
}

Vector3 Camera::PixelSampleSquare(int sample_i, int sample_j) const
{
    auto px = -0.5 +
              recip_sqrt_samples_per_pixel * (sample_i + RandomDouble());
    auto py = -0.5 +
              recip_sqrt_samples_per_pixel * (sample_j + RandomDouble());

    return px * pixel_delta_u + py * pixel_delta_v;
}

Point3 Camera::DefocusDiskSample() const
{
    auto point = RandomPoint3InUnitDisk();
    return camera_center + point[0] * defocus_disk_u + point[1] * defocus_disk_v;
}

Color Camera::RayColor(const Ray &ray, int depth, const Hittable &world) const
{
    HitRecord hit_record;

    if (depth <= 0)
    {
        return Color(0.);
    }

    if (!world.IsHit(ray, Interval(0.0001, _INFINITY), hit_record))
    {
        return background;
    }

    Ray scattered_ray;
    Color attenuation(1.);
    auto color_from_emission = hit_record.material->Emit(hit_record, hit_record.u, hit_record.v, hit_record.intersection_point);

    if (!hit_record.material->Scatter(ray, hit_record, attenuation, scattered_ray))
    {
        return color_from_emission;
    }

    auto on_light = Point3{RandomDouble(213., 343.), 554.9, RandomDouble(227., 332.)};
    auto to_light = on_light - hit_record.intersection_point;
    auto distance_squared = to_light.LengthSquared();
    to_light = UnitVector(to_light);

    if (Dot(to_light, hit_record.normal) < 0)
    {
        return color_from_emission;
    }
    double light_area = (343. - 213.) * (332. - 227.);
    auto light_cosine = std::fabs(to_light.Y());
    if (light_cosine < 0.000001)
    {
        return color_from_emission;
    }

    auto pdf = distance_squared / (light_cosine * light_area);
    scattered_ray = Ray{hit_record.intersection_point, to_light, ray.Time()};

    auto scattering_pdf = hit_record.material->ScatteringPdf(ray, hit_record, scattered_ray);

    auto color_from_scatter = (attenuation * scattering_pdf * RayColor(scattered_ray, depth - 1, world)) / pdf;

    return color_from_emission + color_from_scatter;
}
