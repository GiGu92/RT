#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere() : center(0, 0, 0), radius(1) {};
  sphere(point3 cen, real r) : center(cen), radius(r) {};

  virtual bool hit(const ray& r, real t_min, real t_max, hit_record& rec) const override;

private:
  point3 center;
  real radius;
};

bool sphere::hit(const ray& r, real t_min, real t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  real a = r.direction().length_squared();
  real half_b = dot(oc, r.direction());
  real c = oc.length_squared() - radius * radius;

  real discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  real sqrtd = sqrt(discriminant);
  
  // Find the nearest root that lies in the acceptable range.
  real root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);

  return true;
}