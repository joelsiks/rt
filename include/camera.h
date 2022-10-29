
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class Camera {
public:
    // Camera constructor, vertical fov is in degrees.
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vertical_fov,
           double aspect_ratio) {

        // The angle theta is the angle between the upper and lower part of the
        // viewport.
        auto theta = degress_to_radians(vertical_fov);

        // h is the distance from the z-axis to the top of the viewport.
        auto h = tan(theta / 2);

        // The viewport is the size of the projection plane.
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = cross(w, u);

        m_origin = lookfrom;
        m_horizontal = viewport_width * u;
        m_vertical = viewport_height * v;
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 - w;
    }

    // u and v are values between 0.0 - 1.0.
    Ray get_ray(double s, double t) {
        return Ray(m_origin, m_lower_left_corner + s * m_horizontal +
            t * m_vertical - m_origin);
    }

private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3 m_horizontal;
    Vec3 m_vertical;
};

#endif // CAMERA_H
