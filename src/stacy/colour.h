#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.h"

#include <iostream>

using colour = vec3;

// Standard colours
static colour red = colour(1.0, 0.0, 0.0);
static colour green = colour(0.0, 1.0, 0.0);
static colour blue = colour(0.0, 0.0, 1.0);
static colour white = colour(1.0, 1.0, 1.0);
static colour black = colour(0.0, 0.0, 0.0);

// Based on artistic preference
static colour light_blue = colour(0.5, 0.7, 1.0);
static colour light_green = colour(0.565, 0.933, 0.565);
static colour light_red = colour(0.85, 0.3, 0.3);
static colour grey = colour(0.5, 0.5, 0.5);

constexpr std::string_view inferno_data = "00000401000501010601010802010a02020c02020e03021004031204031405041706041907051b08051d09061f0a07220b07240c08260d08290e092b10092d110a30120a32140b34150b37160b39180c3c190c3e1b0c411c0c431e0c451f0c48210c4a230c4c240c4f260c51280b53290b552b0b572d0b592f0a5b310a5c320a5e340a5f3609613809623909633b09643d09653e0966400a67420a68440a68450a69470b6a490b6a4a0c6b4c0c6b4d0d6c4f0d6c510e6c520e6d540f6d550f6d57106e59106e5a116e5c126e5d126e5f136e61136e62146e64156e65156e67166e69166e6a176e6c186e6d186e6f196e71196e721a6e741a6e751b6e771c6d781c6d7a1d6d7c1d6d7d1e6d7f1e6c801f6c82206c84206b85216b87216b88226a8a226a8c23698d23698f24699025689225689326679526679727669827669a28659b29649d29649f2a63a02a63a22b62a32c61a52c60a62d60a82e5fa92e5eab2f5ead305dae305cb0315bb1325ab3325ab43359b63458b73557b93556ba3655bc3754bd3853bf3952c03a51c13a50c33b4fc43c4ec63d4dc73e4cc83f4bca404acb4149cc4248ce4347cf4446d04545d24644d34743d44842d54a41d74b3fd84c3ed94d3dda4e3cdb503bdd513ade5238df5337e05536e15635e25734e35933e45a31e55c30e65d2fe75e2ee8602de9612bea632aeb6429eb6628ec6726ed6925ee6a24ef6c23ef6e21f06f20f1711ff1731df2741cf3761bf37819f47918f57b17f57d15f67e14f68013f78212f78410f8850ff8870ef8890cf98b0bf98c0af98e09fa9008fa9207fa9407fb9606fb9706fb9906fb9b06fb9d07fc9f07fca108fca309fca50afca60cfca80dfcaa0ffcac11fcae12fcb014fcb216fcb418fbb61afbb81dfbba1ffbbc21fbbe23fac026fac228fac42afac62df9c72ff9c932f9cb35f8cd37f8cf3af7d13df7d340f6d543f6d746f5d949f5db4cf4dd4ff4df53f4e156f3e35af3e55df2e661f2e865f2ea69f1ec6df1ed71f1ef75f1f179f2f27df2f482f3f586f3f68af4f88ef5f992f6fa96f8fb9af9fc9dfafda1fcffa4";
constexpr int inferno_data_step = inferno_data.size() / 6;

// TODO: Don't go from 0 to 256 and then back.
// Assumes degree is between 0 and 1.
colour inferno(double degree) {
  const auto index = std::min(inferno_data_step - 1, std::max(0, int(std::floor(degree * inferno_data_step))));
  // String stored as RRGGBB
  // TODO: Use std::strtoul to deal with raw char* arrays
  std::string colour_hex = std::string(inferno_data.substr(index * 6, 6));
  float r = std::stoul(colour_hex.substr(0, 2).c_str(), nullptr, 16) / 255.0f;
  float g = std::stoul(colour_hex.substr(2, 2).c_str(), nullptr, 16) / 255.0f;
  float b = std::stoul(colour_hex.substr(4, 2).c_str(), nullptr, 16) / 255.0f;
  return colour(r, g, b);
}

void write_colour(std::ostream& out, const colour& pixel_colour) {
  auto r = pixel_colour.x();
  auto g = pixel_colour.y();
  auto b = pixel_colour.z();

  auto linear_to_gamma = [](double v) {
    return (v > 0) ? std::sqrt(v) : 0;
  };

  auto clamp = [](double x) -> double {
    if (x < 0.000) return 0.000;
    if (x > 0.999) return 0.999;
    return x;
  };

  // Translate [0, 1] components into the byte range [0, 255]
  int ir = int(255.999 * clamp(linear_to_gamma(r)));
  int ig = int(255.999 * clamp(linear_to_gamma(g)));
  int ib = int(255.999 * clamp(linear_to_gamma(b)));
  
  // Write out PPM format
  std::cout << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif
