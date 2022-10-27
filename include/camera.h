
#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class Camera {
public:
    Camera() {
        auto aspect_ratio = 16.0 / 9.0;

        // The viewport is the size of the projection plane.
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;

        // Focal length is the distance between the projection plane and the
        // projection point.
        auto focal_length = 1.0;

        m_origin = Point3(0.0, 0.0, 0.0);
        m_horizontal = Vec3(viewport_width, 0.0, 0.0);
        m_vertical = Vec3(0.0, viewport_height, 0.0);
        m_lower_left_corner = m_origin - m_horizontal / 2 - m_vertical / 2 -
                              Vec3(0.0, 0.0, focal_length);
    }

    // u and v are values between 0.0 - 1.0.
    Ray get_ray(double u, double v) {
        return Ray(m_origin, m_lower_left_corner + u * m_horizontal +
                   v * m_vertical - m_origin);
    }

private:
    Point3 m_origin;
    Point3 m_lower_left_corner;
    Vec3 m_horizontal;
    Vec3 m_vertical;
};

#endif // CAMERA_H
