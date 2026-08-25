#include "camera.h"
#include "sphere.h"
#include "world.h"

#include <iostream>

int main() {
  world world;

  // ground 
  world.add(sphere(point3(0,-1000,-1), 1000, metallic(light_green)));

  // little spheres
  for (int a = -15; a < 15; a++) {
    for (int b = -15; b < 15; b++) {
      auto choose_mat = random_double_zero_to_one();
      point3 centre(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((centre - point3(4, 0.2, 0)).length() > 0.9) {
        if (choose_mat < 0.5) {
          // 50% of spheres will be random diffuse colours
          auto col = random_vec3_zero_to_one() * random_vec3_zero_to_one(); //  * colour::random_vec3_zero_to_one();
          world.add(sphere(centre, 0.2, lambertian(col)));
        } else if (choose_mat < 0.90) {
          // 15% of spheres will be metal
          auto col = random_vec3_zero_to_one();
          auto fuzz = random_double(0, 0.7);
          world.add(sphere(centre, 0.2, metallic(col, fuzz)));
        } else {
          // remaining 10% will be glass
          double refraction_glass_sphere = random_double(1.0, 2.0);
          world.add(sphere(centre, 0.2, dielectric(refraction_glass_sphere)));
          world.add(sphere(centre, 0.2, dielectric(/*air*/1.00 / refraction_glass_sphere)));
        }
      }
    }
  }

  // Add a really big metal sphere in the middle.
  world.add(sphere(point3(4, 1, 0), 1.0, metallic(colour(0.7, 0.6, 0.5), 0.0)));


  camera cam(/*camera_centre_=*/point3(15,2,4), /*vfov_=*/20, 16.0 / 9.0, 3840);
  cam.set_using_shading(true);
  cam.set_max_depth(15);
  cam.set_samples_per_pixel(1);

  cam.render(world);

  return 0;
}
