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
 * $Id: float.c,v 1.1 2007-03-25 14:51:08 cruanyes Exp $
 */
#define LIB3DS_EXPORT
#include <lib3ds/float.h>


/*!
 * \defgroup float Floating Point Mathematics
 *
 * \author J.E. Hoffmann <je-h@gmx.net>
 */


/*!
 * \ingroup float
 */
Lib3dsFloat
lib3ds_float_cubic(Lib3dsFloat a, Lib3dsFloat p, Lib3dsFloat q, Lib3dsFloat b, Lib3dsFloat t)
{
  Lib3dsDouble x,y,z,w;   

  x=2*t*t*t - 3*t*t + 1;
  y=-2*t*t*t + 3*t*t;
  z=t*t*t - 2*t*t + t;
  w=t*t*t - t*t;
  return((Lib3dsFloat)(x*a + y*b + z*p + w*q));
}

