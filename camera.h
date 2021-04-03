#pragma once

#include "rtweekend.h"

class camera {
public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, real vfov, real aspect_ratio, real aperture, real focus_dist) {
    real theta = degrees_to_radians(vfov);
    real h = tan(theta / 2.0);
    const real viewport_height = 2.0 * h;
    const real viewport_width = aspect_ratio * viewport_height;

    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    origin = lookfrom;
    horizontal = focus_dist * viewport_width * u;
    vertical = focus_dist * viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

    lens_radius = aperture / 2;
  }

  ray get_ray(real s, real t) const {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    return ray(
      origin + offset,
      lower_left_corner + s * horizontal + t * vertical - origin - offset);
  }

private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  real lens_radius;
};