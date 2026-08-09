Ray Tracing in One Weekend
====================================================================================================

This is my raytracer implementation based on the "Ray Tracing In One Weekend" series.
Here is the current state of my spheres:

| ![foo][out5] | ![RT The Next Week][out5dbg] |
|:----------------------------:|:---------------------------:|
|   Camera with depth projection    |   "naive" screne traversal | 

Implementation
------------------
I am reading and following the [textbook] out-of-order. Here are my plans for what to tackle next:

* [ ] Factor out the camera implementation to debug the warped effect
* [ ] Understand why the colours are different between the two camera approaches
* [ ] Anti-aliasing (I have no idea what this is)
* [ ] Add different sphere materials
* [ ] Cube?
* [ ] Parallel Rust implementation? 

### Files
The files in `src/stacy` act as "checkpoints" along the journey to render 1001 spheres:

- [rt1] is just a shaded plane
- [rt2] uses vec3 but does the same
- [rt3] draws 1 spheres and shades it according using normals
- [rt4] simplifies the sphere equation and draws 3 spheres, which are warped (zoom out makes this obvious). [rt4dbg] attempts to deal with the warping problem by diverging from the book in terms of camera setup.
- [rt5] introduces the concept of the "world" with objects in the world, checks hit intersections and returns the nearest one. [rt5dbg] diverges in terms of camera setup.

The spheres will be improving.

### Running this code
[RayTracing in One Weekend][textbook] uses `cmake` so I am as well. To make these on your own you need `imagemagick` and `cmake` and should just be able to run `./makert <rtnumber>`.

[out5]:            out/rt5.png
[out5dbg]:         out/rt5dbg.png
[rt1]:             src/stacy/ray-tracing1.cc
[rt2]:             src/stacy/ray-tracing2.cc
[rt3]:             src/stacy/ray-tracing3.cc
[rt4]:             src/stacy/ray-tracing4.cc
[rt4dbg]:          src/stacy/ray-tracing4-debug.cc
[rt5]:             src/stacy/ray-tracing5.cc
[rt5dbg]:          src/stacy/ray-tracing5-debug.cc
[textbook]:        https://raytracing.github.io/books/RayTracingInOneWeekend.html
