/*
 * The 3D Studio File Format Library
 * Copyright (C) 1996-2001 by J.E. Hoffmann <je-h@gmx.net>
 * All rights reserved.
 *
 * This program is  free  software;  you can redistribute it and/or modify it
 * under the terms of the  GNU Lesser General Public License  as published by 
 * the  Free Software Foundation;  either version 2.1 of the License,  or (at 
 * your option) any later version.
 *
 * This  program  is  distributed in  the  hope that it will  be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
 * License for more details.
 *
 * You should  have received  a copy of the GNU Lesser General Public License
 * along with  this program;  if not, write to the  Free Software Foundation,
 * Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: camera.c,v 1.1 2007-03-25 14:51:03 cruanyes Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/camera.h>
#include <lib3ds/chunk.h>
#include <lib3ds/io.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <config.h>
#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif


/*!
 * \defgroup camera Cameras
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*!
 * \ingroup camera
 */
Lib3dsCamera*
lib3ds_camera_new(const char *name)
{
  Lib3dsCamera *camera;

  ASSERT(name);
  ASSERT(strlen(name)<64);
  
  camera=(Lib3dsCamera*)calloc(sizeof(Lib3dsCamera), 1);
  if (!camera) {
    return(0);
  }
  strcpy(camera->name, name);
  camera->fov=45.0f;
  return(camera);
}


/*!
 * \ingroup camera 
 */
void
lib3ds_camera_free(Lib3dsCamera *camera)
{
  memset(camera, 0, sizeof(Lib3dsCamera));
  free(camera);
}


/*!
 * \ingroup camera
 */
void
lib3ds_camera_dump(Lib3dsCamera *camera)
{
  ASSERT(camera);
  printf("  name:       %s\n", camera->name);
  printf("  position:   (%f, %f, %f)\n", 
    camera->position[0], camera->position[1], camera->position[2]);
  printf("  target      (%f, %f, %f)\n", 
    camera->target[0], camera->target[1], camera->target[2]);
  printf("  roll:       %f\n", camera->roll);
  printf("  fov:        %f\n", camera->fov);
  printf("  see_cone:   %s\n", camera->see_cone ? "yes" : "no");
  printf("  near_range: %f\n", camera->near_range);
  printf("  far_range:  %f\n", camera->near_range);
  printf("\n");
}


/*!
 * \ingroup camera
 */
Lib3dsBool
lib3ds_camera_read(Lib3dsCamera *camera, Lib3dsIo *io)
{
  Lib3dsChunk c;
  Lib3dsWord chunk;

  if (!lib3ds_chunk_read_start(&c, LIB3DS_N_CAMERA, io)) {
    return(LIB3DS_FALSE);
  }
  {
    int i;
    for (i=0; i<3; ++i) {
      camera->position[i]=lib3ds_io_read_float(io);
    }
    for (i=0; i<3; ++i) {
      camera->target[i]=lib3ds_io_read_float(io);
    }
  }
  camera->roll=lib3ds_io_read_float(io);
  {
    float s;
    s=lib3ds_io_read_float(io);
    if (fabs(s)<LIB3DS_EPSILON) {
      camera->fov=45.0;
    }
    else {
      camera->fov=2400.0f/s;
    }
  }
  lib3ds_chunk_read_tell(&c, io);
  
  while ((chunk=lib3ds_chunk_read_next(&c, io))!=0) {
    switch (chunk) {
      case LIB3DS_CAM_SEE_CONE:
        {
          camera->see_cone=LIB3DS_TRUE;
        }
        break;
      case LIB3DS_CAM_RANGES:
        {
          camera->near_range=lib3ds_io_read_float(io);
          camera->far_range=lib3ds_io_read_float(io);
        }
        break;
      default:
        lib3ds_chunk_unknown(chunk);
    }
  }
  
  lib3ds_chunk_read_end(&c, io);
  return(LIB3DS_TRUE);
}


/*!
 * \ingroup camera
 */
Lib3dsBool
lib3ds_camera_write(Lib3dsCamera *camera, Lib3dsIo *io)
{
  Lib3dsChunk c;

  c.chunk=LIB3DS_N_CAMERA;
  if (!lib3ds_chunk_write_start(&c,io)) {
    return(LIB3DS_FALSE);
  }

  lib3ds_io_write_vector(io, camera->position);
  lib3ds_io_write_vector(io, camera->target);
  lib3ds_io_write_float(io, camera->roll);
  if (fabs(camera->fov)<LIB3DS_EPSILON) {
    lib3ds_io_write_float(io, 2400.0f/45.0f);
  }
  else {
    lib3ds_io_write_float(io, 2400.0f/camera->fov);
  }

  if (camera->see_cone) {
    Lib3dsChunk c;
    c.chunk=LIB3DS_CAM_SEE_CONE;
    c.size=6;
    lib3ds_chunk_write(&c, io);
  }
  {
    Lib3dsChunk c;
    c.chunk=LIB3DS_CAM_RANGES;
    c.size=14;
    lib3ds_chunk_write(&c, io);
    lib3ds_io_write_float(io, camera->near_range);
    lib3ds_io_write_float(io, camera->far_range);
  }

  if (!lib3ds_chunk_write_end(&c,io)) {
    return(LIB3DS_FALSE);
  }
  return(LIB3DS_TRUE);
}


/*!

\typedef Lib3dsCamera
  \ingroup camera
  \sa _Lib3dsCamera

*/
