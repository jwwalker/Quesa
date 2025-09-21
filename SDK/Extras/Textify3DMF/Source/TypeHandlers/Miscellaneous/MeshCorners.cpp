/*
 *  Normal.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/21/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "MeshCorners.h"

MeshCorners::MeshCorners()
  : TypeHandler( 'crnr', "MeshCorners" )
{
}

void  MeshCorners::Process( size_t inStartOffset,
                  size_t inEndOffset )
{
  //  TODO: DataLengthExceptions

  size_t offset = inStartOffset;
  
  Out() << Indent() << Name() << " (\n";
  int numCorners = FetchUInt32(offset); offset += 4;
  Out() << Indent(1) << numCorners << " # numCorners\n\n";
  for (int i = 0; i < numCorners; i++)
  {
    int vertexIndex = FetchUInt32(offset); offset += 4;
    int numFaces = FetchUInt32(offset); offset += 4;
    Out() <<
      Indent(1) << "# Corner " << i << "\n" <<
      Indent(1) << vertexIndex << " # vertexIndex\n" <<
      Indent(1) << numFaces << " # numFaces\n" << Indent(1);
    for (int j = 0; j < numFaces; j++)
    {
      int faceIndex = FetchUInt32(offset); offset += 4;
      Out() << faceIndex << ((j + 1 == numFaces) ? " # face indices\n\n" : " ");
    }
  }
  Out() << " )\n";
}
