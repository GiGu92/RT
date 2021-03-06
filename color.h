#pragma once

#include "vec3.h"

#include <iostream>

void write_color(unsigned char out[3], const color& pixel_color, int samples_per_pixel) {
  real r = pixel_color.x();
  real g = pixel_color.y();
  real b = pixel_color.z();

  // Divide the color by the number of samples and gamma-correct for gamma=2.0.
  auto scale = 1.0 / samples_per_pixel;
  r = sqrt(scale * r);
  g = sqrt(scale * g);
  b = sqrt(scale * b);

  // Write the translated [0,255] value of each color component.
  out[0] = static_cast<unsigned char>(256 * clamp(r, 0.0, 0.999));
  out[1] = static_cast<unsigned char>(256 * clamp(g, 0.0, 0.999));
  out[2] = static_cast<unsigned char>(256 * clamp(b, 0.0, 0.999));
}