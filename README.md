Ray Tracing in One Weekend
====================================================================================================

This is my raytracer implementation based on the "Ray Tracing In One Weekend" series.
Here is the current state of my spheres:

| ![foo][out6_far] | ![RT The Next Week][out6_near] | ![foo][out6_side] | ![RT The Next Week][out6_top_left] |
|:----------------------------:|:---------------------------:|:----------------------------:|:---------------------------:|
|   Spheres far    |  Spheres near | spheres from the side | spheres from the top | 

Implementation
------------------
I am reading and following the [textbook] out-of-order. Here are my plans for what to tackle next:

* [x] Factor out the camera implementation to debug the warped effect
  * see [rt5] and [rt5dbg]
* [ ] Understand why the colours are different between the two camera approaches
* [ ] Add a light source and shade accordingly
* [ ] Add different sphere materials
* [ ] Anti-aliasing (I have no idea what this is)
* [ ] Cube?
* [ ] Parallel Rust implementation?

### Files
The files in `src/stacy` act as "checkpoints" along the journey to render 1001 spheres:

- [rt1] is just a shaded plane
- [rt2] uses vec3 but does the same
- [rt3] draws 1 spheres and shades it according using normals
- [rt4] simplifies the sphere equation and draws 3 spheres, which are warped (zoom out makes this obvious). [rt4dbg] attempts to deal with the warping problem by diverging from the book in terms of camera setup.
- [rt5] introduces the concept of the "world" with objects in the world, checks hit intersections and returns the nearest one. [rt5dbg] diverges in terms of camera setup.
- [rt6] factors out the camera and applies vertial-field-of-view transforms

The spheres will be improving.

### Running this code
[RayTracing in One Weekend][textbook] uses `cmake` so I am as well. To make these on your own you need `imagemagick` and `cmake` and should just be able to run `./makert <rtnumber>`.

[out5]:            out/rt5.png
[out5dbg]:         out/rt5dbg.png
[out6_near]:       out/rt6_near.png
[out6_far]:        out/rt6_far.png
[out6_side]:       out/rt6_side.png
[out6_top_left]:   out/rt6_top_left.png
[rt1]:             src/stacy/ray-tracing1.cc
[rt2]:             src/stacy/ray-tracing2.cc
[rt3]:             src/stacy/ray-tracing3.cc
[rt4]:             src/stacy/ray-tracing4.cc
[rt4dbg]:          src/stacy/ray-tracing4-debug.cc
[rt5]:             src/stacy/ray-tracing5.cc
[rt5dbg]:          src/stacy/ray-tracing5-debug.cc
[rt6]:             src/stacy/ray-tracing6.cc
[textbook]:        https://raytracing.github.io/books/RayTracingInOneWeekend.html
