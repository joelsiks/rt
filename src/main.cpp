
#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

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
        Point3 target = rec.point + rec.normal + random_unit_vector();
        return 0.5 * ray_color(Ray(rec.point, target - rec.point), world,
                depth - 1);
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
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

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