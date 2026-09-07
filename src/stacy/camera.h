#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "colour.h"
#include "world.h"

#include <algorithm>
#include <optional>
#include <thread>

enum ColourMode {
  Scene,
  NormalsOnly,
  NumBounces,
};

struct ray_results {
  colour pixel_colour;
  uint num_hittests;
};

class camera {
 public:
  camera(const point3& camera_centre, double vfov, double aspect_ratio = 16.0 / 9.0, int img_width = 1280) : camera_centre_(camera_centre), vfov_(vfov), aspect_ratio_(aspect_ratio), img_width_(img_width) {
    img_height_ = int(img_width / aspect_ratio);
    total_num_pixels_ = (img_height_ * img_width_);

    auto focal_length = (camera_centre_ - scene_centre).length();
    auto theta = degrees_to_radians(vfov_);
    auto h = std::tan(theta/2);
    auto viewport_height = 2 * h * focal_length;
    auto viewport_width = viewport_height * (double(img_width)/img_height_);

    w = unit_vector(camera_centre_ - scene_centre);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // These vectors represent viewport traversal deltas in the horizontal and vertical
    vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
    vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical deltas, which are also vectors
    pixel_delta_u = viewport_u / img_width;
    pixel_delta_v = viewport_v / img_height_;

    vec3 Q = camera_centre_ - (focal_length * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = Q + (0.5 * (pixel_delta_u + pixel_delta_v));

    // Anti-aliassing weighs the pixel colour by the number of sample rays per pixel.
    pixels_colour_scale_ = 1.0 / samples_per_pixel_;

    // Reserve space for results.
    results_.reserve(img_width_ * img_height_);
    num_hits_.reserve(img_width_ * img_height_);
    num_hits_.resize(img_width_ * img_height_);
  }


  void render(const world& world) {
    // Write PPM file
    std::cout << "P3\n" << img_width_ << ' ' << img_height_ << "\n255\n";

    unsigned int num_cores = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_cores);


    std::atomic<int> index_counter{0};
    auto worker_fn = [&](){
      while (true) {
        int n = index_counter.fetch_add(1, std::memory_order_relaxed);
        if (n > total_num_pixels_)
          break;

        colour pixel_colour = black;
        double num_hits = 0;
        int w = n % img_width_;
        int h = int(n / img_width_); 
        for (int sample = 0; sample < samples_per_pixel_; sample++) {
          // std::clog << "\r Pixels remaining: " << (total_num_pixels_ - ((h * img_width_) + w)) <<  " \n "; // << std::flush;
          ray r = get_ray(w, h);
          ray_results pixel_info = ray_colour(r, max_depth_, world);
          pixel_colour += pixels_colour_scale_ * pixel_info.pixel_colour;
          num_hits = double(pixel_info.num_hittests);
        }
        results_[n] = pixel_colour;
        num_hits_[n] = num_hits;
      }
    };

    for (unsigned int i = 0; i < num_cores; ++i) {
        threads.emplace_back(worker_fn);
    }
    for (auto& thread : threads) {
        thread.join();
    }

    if (recolour_using_debug_info_) {
      auto max_it = std::max_element(num_hits_.begin(), num_hits_.end());
      const double max_val = *max_it;
      std::transform(num_hits_.begin(), num_hits_.end(), results_.begin(), [max_val](double result) {
        return inferno(result / max_val);
      });
    }

    std::clog << std::flush;
    for (int h = 0; h < img_height_; h++) {
      // std::clog << "\r Scanlines remaining: " << (img_height_ - h) <<  " " << std::flush;
      for (int w = 0; w < img_width_; w++) {
        write_colour(std::cout, results_[(h * img_width_) + w]);
      }
    }
    // std::clog << "\r Done! \n";
  }

  void set_samples_per_pixel(int samples_per_pixel) {
    samples_per_pixel_ = samples_per_pixel;
    pixels_colour_scale_ = 1.0 / samples_per_pixel_;
  }

  void set_max_depth(int max_depth) {
    max_depth_ = max_depth;
  }

  void set_colour_mode(ColourMode mode) {
    mode_ = mode;
  }

  void set_recolour_using_debug_info(bool recolour_using_debug_info){
    recolour_using_debug_info_ = recolour_using_debug_info;
  }

  // TODO: Implement camera pan
  point3 scene_centre   = point3(0,0,-1);  // Point camera is looking at.
  vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction.

 private:

  // Sample a random point and tracing a ray through that point.
  ray get_ray(int pixel_w, int pixel_h) const {
    // Offset is a random vector in the [-.5,-.5]-[+.5,+.5] unit square.
    vec3 offset = sample_unit_square();
    if (samples_per_pixel_ == 1) {
      offset = vec3(0,0,0);
    }
    vec3 pixel_sample = pixel00_loc + ((pixel_w + offset.x())* pixel_delta_u) + ((pixel_h + offset.y()) * pixel_delta_v);
    vec3 ray_direction = pixel_sample - camera_centre_;
    return ray(/* ray origin = */camera_centre_, ray_direction);
  }

  vec3 stacy_lerp(vec3 start_value, vec3 end_value, float a) {
    return (1.0-a)*start_value + a*end_value;
  }

  // TODO: Clean up this function, it's getting messy.
  ray_results ray_colour(const ray& r, int max_depth, const world& world) {
    if (!max_depth) {
      return ray_results{white, 0};
    }

    std::optional<hit_record> record = world.hit(r, std::numeric_limits<double>::infinity());
    if (record.has_value()) {
      auto rec_value = record.value();
      auto bounce = ray_colour(rec_value.scattered, max_depth-1, world);
      switch (mode_) {
        case ColourMode::Scene:
          return ray_results{rec_value.attenuation * bounce.pixel_colour, rec_value.num_hit_tests + bounce.num_hittests};
          break;
        case ColourMode::NormalsOnly: // No bounces
          return ray_results{0.5 * (record.value().normal + white), rec_value.num_hit_tests};
          break;
        case ColourMode::NumBounces:
          return ray_results{bounce.pixel_colour, rec_value.num_hit_tests + bounce.num_hittests};
          break;
      }
    }

    // Background gradient
    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0);

    if (mode_ == ColourMode::NumBounces) {
      double x = double(double(max_depth_ - max_depth)/double(max_depth_));
      return ray_results{x * white, 0};
    }
    return ray_results{stacy_lerp(blue, white, a), 0};
  }

  // Camera setup
  point3 camera_centre_;
  double vfov_; // Vertical field of view
  ColourMode mode_ = ColourMode::Scene;

  // Image dimensions
  double aspect_ratio_;
  int img_width_;
  int img_height_; // derived from width
  int total_num_pixels_;
  // Defaults are low easy debug.
  // TODO: Make a config constructor w/ debug & prod modes.
  int samples_per_pixel_ = 1;
  int max_depth_ = 5;
  double pixels_colour_scale_ = 1.0 / samples_per_pixel_; // derived from samples_per_pixel;

  // Viewport setup
  vec3 pixel00_loc;
  vec3 pixel_delta_u;
  vec3 pixel_delta_v;
  vec3 u, v, w;  // Camera frame basis vectors

  // Feature flags TODO remove
  bool recolour_using_debug_info_;

  // Paralellization
  std::vector<colour> results_;
  std::vector<double> num_hits_;
};

#endif