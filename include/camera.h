
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class Camera {
public:
    // Camera constructor, vertical fov is in degrees.
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vertical_fov,
           double aspect_ratio, double aperture, double focus_dist) {

        // The angle theta is the angle between the upper and lower part of the
        // viewport.
        auto theta = degress_to_radians(vertical_fov);

        // h is the distance from the z-axis to the top of the viewport.
        auto h = tan(theta / 2);

        // The viewport is the size of the projection plane.
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        m_w = unit_vector(lookfrom - lookat);
        m_u = unit_vector(cross(vup, m_w));
        m_v = cross(m_w, m_u);

        m_origin = lookfrom;
        m_horizontal = focus_dist * viewport_width * m_u;
        m_vertical = focus_dist * viewport_height * m_v;
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 -
            focus_dist * m_w;

        m_lens_radius = aperture / 2;
    }

    // u and v are values between 0.0 - 1.0.
    Ray get_ray(double s, double t) {
        Vec3 rd = m_lens_radius * random_in_unit_disk();
        Vec3 offset = m_u * rd.x() + m_v * rd.y();

        return Ray(m_origin + offset, m_lower_left_corner + s * m_horizontal +
            t * m_vertical - m_origin - offset);
    }

private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3 m_horizontal;
    Vec3 m_vertical;

    Vec3 m_u, m_v, m_w;
    double m_lens_radius;
};

#endif // CAMERA_H
