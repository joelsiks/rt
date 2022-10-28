
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord {
    double t;
    Point3 point;

    // The normal should be unit lenght (normed).
    Vec3 normal;

    bool front_face;

    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const = 0;
};

#endif // HITTABLE_H