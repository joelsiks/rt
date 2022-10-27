
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 center, double radius) : m_center(center), m_radius(radius) {};

    virtual bool hit(
        const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

private:
    Point3 m_center;
    double m_radius;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.origin() - m_center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - m_radius * m_radius;

    // Expression under the square root of the PQ-formula (eng ver). This tells
    // us how many real solutions the quadratic function has.
    auto discriminant = half_b * half_b - a * c;

    if(discriminant < 0) {
        return false;
    }

    auto sqrtd = sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;

    // Find the nearest root that lies in the acceptable t range.
    if(root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if(root < t_min || root > t_max) {
            return false;
        }
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    Vec3 outward_normal = (rec.point - m_center) / m_radius;
    rec.set_face_normal(r, outward_normal);

    return true;
};

#endif // SPHERE_H