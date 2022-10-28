
#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct HitRecord;

class Material {
public:
    virtual bool scatter(const Ray &r_in, const HitRecord& rec,
            Color& attenuation, Ray& scattered) const = 0;
};

// Lambertian reflection is a diffuse relection, relecting at random angles
// rather than just one angle.
class Lambertian : public Material {
public:
    Lambertian(const Color& albedo) : m_albedo(albedo) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
            Color& attenuation, Ray& scattered) const override {

        auto scatter_direction = rec.normal + random_unit_vector();

        // Catch bad scatter direction if the random_unit_vector were to be
        // exactly (or almost) opposite to the normal.
        if(scatter_direction.near_zero()) {
            scatter_direction = rec.normal;
        }

        scattered = Ray(rec.point, scatter_direction);
        attenuation = m_albedo;

        return true;
    }

private:
    Color m_albedo;
};

// The reflection of metal is specular (or regular) in the sense that it is
// mirror-like and predictable.
class Metal : public Material {
public:

    Metal(const Color& albedo, double fuzz)
        : m_albedo(albedo), m_fuzz(fuzz < 1 ? fuzz : 1) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
            Color& attenuation, Ray& scattered) const override {

        auto reflect_direction = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.point,
                        reflect_direction + m_fuzz * random_in_unit_sphere());
        attenuation = m_albedo;

        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    Color m_albedo;
    double m_fuzz;
};

#endif // MATERIAL_H
