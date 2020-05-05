# README #

Quesa is a 3D rendering and scene graph library for macOS and Windows, with a BSD license.  It was begun way back in the year 2000 as a re-implementation of Apple's QuickDraw 3D APIs, using OpenGL 2.  Rendering was recently upgraded to OpenGL 3.2.  One can still get a pretty good understanding of how it works, such as the use of reference counted objects and the 3DMF object file format, in [Apple's QuickDraw 3D manual](https://jwwalker.com/downloads/qd3d-manual.pdf).

## Authorship

The Quesa project was begun by Dair Grant.  At its period of maximum activity, it was also administered by José Cruanyes, Roger Holmes, Kevin Matthews, Joe Strout, and James W. Walker, with contributions from a few others.  Now it is solely maintained by James W. Walker.

## Recent Work and Future Plans

### jwwalker:
Most recently, I have added all-seeing and fisheye cameras.  See the API documentation,
or the documentation comments in QuesaCamera.h.

In the near term, I plan to contribute code to import and export 3D models in Collada (.dae) format.

In more of a "hopes and dreams" category, it would be nice to support other rendering APIs such as Vulkan or Metal.

### h-haris:
The Controller and Tracker APIs to support 3D devices are reintroduced.
