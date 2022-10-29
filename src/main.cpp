
#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include "camera.h"

#include <iostream>

Color ray_color(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth <= 0) {
        return Color(0.0, 0.0, 0.0);
    }

    // If the ray hits something in the world, color the ray according to the
    // object's normal.
    if(world.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;

        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }

        return Color(0.0, 0.0, 0.0);
    }

    // If the ray doesn't hit a object in the world, draw the backdrop blend.
    Vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int ray_bounce_limit = 50;

    // World
    HittableList world;

    auto material_ground = std::make_shared<Lambertian>(Color(0.4, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.5);
    auto material_right  = std::make_shared<Metal>(Color(0, 0.1459, 0.81960), 0.0);

    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),  -0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    Camera cam(Point3(-2, 2, 1), Point3(0, 0, -1), Vec3(0, 1, 0), 50.0,
        aspect_ratio);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = image_height - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < image_width; i++) {
            Color pixel_color(0.0, 0.0, 0.0);

            // The pixel color for each pixel is made up of blend of rays in
            // close proximity to the actual pixel ray.
            for(int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);

                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, ray_bounce_limit);
            }

            // pixel_color gets scaled down by the samples_per_pixel here
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    return 0;
}