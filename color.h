#pragma once

#include "vec3.h"

#include <iostream>

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {

  // Divide the color by the number of samples
  float scale = 1.f / samples_per_pixel;
  pixel_color *= scale;

  // Write the translated [0,255] value of each color component.
  out
    << static_cast<int>(255.999 * pixel_color.x()) << ' '
    << static_cast<int>(255.999 * pixel_color.y()) << ' '
    << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}