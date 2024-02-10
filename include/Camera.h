#pragma once

#include "global.h"
#include "Color.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Material.h"

#include <chrono>

class Camera
{
public:
    void Render(const HittableList &world);

private:
    void Initialize();

    Ray GetRay(int i, int j) const;

    Vector3 SampleRandomOffset() const;

    Point3 DefocusDiskSample() const;

    Color RayColor(const Ray &ray, int depth, const Hittable &world) const;

public:
    double aspect_ratio = 1.;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    double vertical_fov = 90.;
    Color background;

    Point3 look_from = Point3(0.);
    Point3 look_at = Point3(0., 0., -1.);
    Vector3 view_up = Vector3(0., 1., 0.);

    double defocus_angle = 0.;
    double focus_distance = 1.;

private:
    int image_height = 0;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;
    Point3 pixel00_loc;
    Point3 camera_center;

    Vector3 u, v, w;

    Vector3 defocus_disk_u, defocus_disk_v;
};
