
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

    virtual bool scatter(const Ray& r_in __attribute__((unused)), const HitRecord& rec,
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

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : m_ir(index_of_refraction) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec,
                         Color& attenuation, Ray& scattered) const override {

        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / m_ir) : m_ir;

        Vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if(cannot_refract ||
                reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.point, direction);
        return true;
    }

private:
    double m_ir; // Index of refraction

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1 - cosine),5);
    }
};

#endif // MATERIAL_H
