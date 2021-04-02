#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <Shlwapi.h> // for ShellExecute

color ray_color(const ray& r, const hittable& world) {
  hit_record rec;
  if (world.hit(r, 0, infinity, rec))
    return 0.5f * (rec.normal + color(1, 1, 1));
  vec3 unit_direction = unit_vector(r.direction());
  float t = 0.5f * (unit_direction.y() + 1.0f);
  return (1.0f - t) * color(1.f, 1.f, 1.f) + t * color(.5f, .7f, 1.f);
}

int main() {

  // Image
  const float aspect_ratio = 16.f / 9.f;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5f));
  world.add(make_shared<sphere>(point3(0, -100.5f, -1), 100.f));

  // Camera
  float viewport_height = 2.0;
  float viewport_width = aspect_ratio * viewport_height;
  float focal_length = 1.0;
  point3 origin(0, 0, 0);
  vec3 horizontal(viewport_width, 0, 0);
  vec3 vertical(0, viewport_height, 0);
  point3 lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
  
  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    int line = image_height - 1 - j;
    int percent = (int)roundf((line / float(image_height)) * 100.f);
    std::cerr << "\rScanlines: " << line << "/" << image_height << " (" << percent << "%)" << std::flush;
    for (int i = 0; i < image_width; ++i) {
      float u = float(i) / (image_width - 1);
      float v = float(j) / (image_height - 1);
      ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
      color pixel_color = ray_color(r, world);
      write_color(std::cout, pixel_color);
    }
  }

  std::cerr << "\nDone.\n";

  ShellExecute(0, 0, L"image.ppm", 0, 0, SW_SHOW);
}