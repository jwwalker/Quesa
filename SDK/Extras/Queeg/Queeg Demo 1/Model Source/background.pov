//		POV-Ray file for generating a background map.
//
//      This generates all six images at once and in the correct order,
//      if you set the 'clock' variable to go from 1 to 6 (in 6 frames) 
//      in the animation settings panel.

#version 3.1;

#include "colors.inc"

global_settings
{
  assumed_gamma 1.0
}

// ----------------------------------------
#declare campos = <0.0, 0.5, 0.0>
camera
{
  location  <0.0, 0.5, 0.0>
  up            y
  right     x

#switch (clock)
  #case (1)
        look_at campos +x
  #break
  #case (2)
        look_at campos -z
  #break
  #case (3)
        look_at campos -x
  #break
  #case (4)
        look_at campos +z
  #break
  #case (5)
        look_at campos +y
  #break
  #case (6)
        look_at campos -y
  #break
#end
  angle 91.35873
}

// ----------------------------------------

// light (should take care to make this match the real-time 3D light)
light_source
{
  0*x // light's position (translated below)
  color red 1.0  green 1.0  blue 1.0  // light's color
  translate <-30, 30, -30>
}

// sky
sky_sphere
{
  pigment
  {
    gradient y
    color_map { [0.0 color rgb <0.8,0.8,1.0>] [1.0 color blue 0.6] }
  }
  pigment {
   bozo
   turbulence 0.65
   octaves 6
   omega 0.7
   lambda 2
   color_map {
       [0.0 0.1 color rgb <0.85, 0.85, 0.85>
                color rgb <0.75, 0.75, 0.75>]
       [0.1 0.5 color rgb <0.75, 0.75, 0.75>
                color rgbt <1, 1, 1, 1>]
       [0.5 1.0 color rgbt <1, 1, 1, 1>
                color rgbt <1, 1, 1, 1>]
      }
      scale <0.2, 0.3, 0.2>
  }
}

// ground
plane
{
  y, -10
        texture {
          pigment { color rgb <0.8, 0.8, 0.4> }
          normal {crackle 0.5 turbulence 0.2 scale 0.5}
        }
}

