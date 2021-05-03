#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <chrono>
#include <Shlwapi.h> // for ShellExecute

static int num_rays = 0;

color ray_color(const ray& r, const hittable& world, int depth) {

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
    return color(0, 0, 0);

  num_rays++;

  hit_record rec;
  if (world.hit(r, 0.001, infinity, rec)) {
    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1);
    return color(0, 0, 0);
  }
  vec3 unit_direction = unit_vector(r.direction());
  real t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1, 1, 1) + t * color(.5, .7, 1.);
}

void random_scene(hittable_list& world) {
  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      real choose_mat = random_real();
      point3 center(a + 0.9 * random_real(), 0.2, b + 0.9 * random_real());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          color albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else if (choose_mat < 0.95) {
          // metal
          color albedo = color::random(0.5, 1);
          real fuzz = random_real(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
}

void simple_scene(hittable_list& world) {
  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dielectric>(1.5);
  auto material_right = make_shared<metal>(color(0.7, 0.6, 0.2), 0.0);

  world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
  world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
}

int main() {

  // Image

  const real aspect_ratio = 3.0 / 2.0;
  const int image_width = 400;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  // World

  hittable_list world;
  //random_scene(world);
  simple_scene(world);

  // Camera

  point3 lookfrom(13, 2, 3);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  real dist_to_focus = 10;
  real aperture = 0.1;
  camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

  // Render

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  auto print_progress = [](int done, int all) {
    int percent = (int)round((done / real(all)) * 100);
    std::cerr << "\rScanlines: " << done << "/" << all << " (" << percent << "%)" << std::flush;
  };

  auto start_time = std::chrono::high_resolution_clock::now();

  for (int j = image_height - 1; j >= 0; --j) {
    print_progress(image_height - 1 - j, image_height);
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        real u = real(i + random_real()) / (image_width - 1);
        real v = real(j + random_real()) / (image_height - 1);
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);

      }
      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
  print_progress(image_height, image_height);

  auto finish_time = std::chrono::high_resolution_clock::now();
  real seconds = (finish_time - start_time).count() * 1e-9;

  std::cerr << "\nDone " << num_rays << " rays in " << seconds << " seconds."
    << " (" << num_rays * 1e-6 / seconds << " Mray/s)";

  getchar();

  ShellExecute(0, 0, L"image.ppm", 0, 0, SW_SHOW);
};