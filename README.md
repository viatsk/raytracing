Ray Tracing (ongoing)
====================================================================================================

This is my raytracer implementation based on the "Ray Tracing In One Weekend" series.
Here is the current state of my spheres:

### Metallic:

| ![foo][out8_all]  | ![foo][outfinal]  |
|:---------------------------:|:---------------------------:|
|Three metallic spheres (no fuzz)| Showing off all interactions |

### Dielectric:
| ![foo][out9_glass_on_metal] | ![foo][out10] |
|:---------------------------:|:---------------------------:|
|Dielectric sphere on metal|Glass (two-layer dielectric), diffuse, and fuzzy metal|


### And here are some arbitrary camera angles I was proud of (as of rt6): 

| ![foo][out6_far] | ![RT The Next Week][out6_near] | ![foo][out6_side] | ![RT The Next Week][out6_top_left] |
|:----------------------------:|:---------------------------:|:----------------------------:|:---------------------------:|
|   Spheres far    |  Spheres near | spheres from the side | spheres from the top | 

## Next Steps
I ~~am~~ was reading and following this [textbook] [and it's follow-up][textbook2] out-of-order, but the spheres are taking too long to render to iterate quickly so my plans are to tackle performance bottlenecks. Roughly, this looks like:

### Performance
  * [ ] Add as many threads as there are cores on machine
  * [ ] Profile
  * [ ] **WIP**: Move away from virtual dispatch
      - Cool part: Implement interface concept using C++20 `contract`, `std::varient` and `std::visit` rather than inheriting from an abstract class
  * [ ] Understand stratified sampling (?)
  * [ ] Benchmark different ways of randomly generating unit spheres 

### Features 
  * [x] Factor out the camera implementation to debug the warped effect
    * see [rt5] and [rt5dbg]
  * [x] Understand why the colours are different between the two camera approaches
  * [x] Add a diffuse lighting and shade accordingly
  * [x] Add different sphere materials
  * [x] Anti-aliasing
  * [ ] Understand cosine normal sampling
  * [ ] Feature flag to disable reflectance in dielectric materials
  * [ ] Cube?
  * [ ] Defocus blur 
  * [x] ~~Parallel Rust implementation?~~

### Files
The files in `src/stacy` act as "checkpoints" along the journey to render 1001 spheres:

- [rt1] is just a shaded plane
- [rt2] uses vec3 but does the same
- [rt3] draws 1 spheres and shades it according using normals
- [rt4] simplifies the sphere equation and draws 3 spheres, which are warped (zoom out makes this obvious). [rt4dbg] attempts to deal with the warping problem by diverging from the book in terms of camera setup.
- [rt5] introduces the concept of the "world" with objects in the world, checks hit intersections and returns the nearest one. [rt5dbg] diverges in terms of camera setup.
- [rt6] factors out the camera and applies vertial-field-of-view transforms
- [rt7] tries to introduce diffuse lighting and shadow interactions between the spheres, currently breaking camera.
- [rt8] renders metal spheres
- [rt9] adds dielectric spheres. when the metal fuzz factor is 0, they act like spherical mirrors
- [rt10] renders all three materials in one scene

The spheres will be improving.

### Running this code
[RayTracing in One Weekend][textbook] uses `cmake` so I am as well. To make these on your own you need `imagemagick` and `cmake` and should just be able to run `./makert <rtnumber>`.

[out5]:            out/rt5.png
[out5dbg]:         out/rt5dbg.png
[out6_near]:       out/rt6_near.png
[out6_far]:        out/rt6_far.png
[out6_side]:       out/rt6_side.png
[out6_top_left]:   out/rt6_top_left.png
[out7_FIXED]:      out/rt7_lotsofpixels_onesphere.png
[out7_FIXED3]:     out/rt7_lotsofpixels.png
[out7_side]:       out/rt7_lotsofpixels_side.png
[out8_all]:        out/rt8_maxdepth50_all.png
[out9_glass_on_metal]: out/rt9_highfidelity_onesphere.png
[out10]:          out/rt10_no_reflectance.jpg
[outfinal]:          out/rtfinal.png
[rt1]:             src/stacy/ray-tracing1.cc
[rt2]:             src/stacy/ray-tracing2.cc
[rt3]:             src/stacy/ray-tracing3.cc
[rt4]:             src/stacy/ray-tracing4.cc
[rt4dbg]:          src/stacy/ray-tracing4-debug.cc
[rt5]:             src/stacy/ray-tracing5.cc
[rt5dbg]:          src/stacy/ray-tracing5-debug.cc
[rt6]:             src/stacy/ray-tracing6.cc
[rt7]:             src/stacy/ray-tracing7.cc
[rt8]:             src/stacy/ray-tracing8.cc
[rt9]:             src/stacy/ray-tracing9.cc
[rt10]:            src/stacy/ray-tracing10.cc
[textbook]:        https://raytracing.github.io/books/RayTracingInOneWeekend.html
[textbook2]:       https://raytracing.github.io/books/RayTracingTheNextWeek.html
