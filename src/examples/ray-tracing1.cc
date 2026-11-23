
#include <iostream>

int main() {

  // Image
  int img_width = 256;
  int img_height = 256;

  // Render
  std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (int h = 0; h < img_height; h++) {
    std::clog << "\r Scanlines remaining: " << (img_height - h) <<  " " << std::flush;
      for (int w = 0; w < img_width; w++) {
        auto r = double (w) / (img_width -1);
        auto g = double (h) / (img_height - 1);
        auto b = 0.0;

        int ir = int(255.999 * r);
        int ig = int(255.999 * g);
        int ib = int(255.999 * b);

        std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }

  std::clog << "\r Done! \n";
  return 0;
}


