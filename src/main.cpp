
#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include "camera.h"

#include <iostream>

HittableList random_scene() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for(int i = -11; i < 11; i++) {
        for(int j = -11; j < 11; j++) {
            auto choose_mat = random_double();
            Point3 center(i + 0.9 * random_double(), 0.2, j + 0.9 * random_double());

            if((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // Glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

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
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int ray_bounce_limit = 50;

    // World
    HittableList world = random_scene();

    auto material_ground = std::make_shared<Lambertian>(Color(0.4, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.5);
    auto material_right  = std::make_shared<Metal>(Color(0, 0.1459, 0.81960), 0.0);

    world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),  -0.5, material_left));
    world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    Point3 lookfrom(13.0, 2.0, 3.0);
    Point3 lookat(0.0, 0.0, 0.0);
    Vec3 vup(0, 1, 0);
    double fov_deg = 50;
    auto aperture = 0.1;
    auto dist_to_focus = 10.0;

    Camera cam(lookfrom, lookat, vup, fov_deg, aspect_ratio, aperture,
            dist_to_focus);

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