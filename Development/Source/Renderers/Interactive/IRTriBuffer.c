/*  NAME:
        IRTriBuffer.c

    DESCRIPTION:
        Quesa interactive renderer triangle buffer.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRTriBuffer.h"
#include "IRUpdate.h"

#include "GLUtils.h"



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRTriBuffer_Initialize : Initialise the triangle buffer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRTriBuffer_Initialize(TQ3InteractiveData *instanceData)
{


	// Initialise our state
	instanceData->triBufferSlab = Q3SlabMemory_New(sizeof(TQ3FVertex3D), 0, NULL);
	if (instanceData->triBufferSlab == NULL)
		return(kQ3Failure);
	
	return(kQ3Success);
}





//=============================================================================
//      IRTriBuffer_Terminate : Terminate the triangle buffer.
//-----------------------------------------------------------------------------
void
IRTriBuffer_Terminate(TQ3InteractiveData *instanceData)
{


	// Release our state
	Q3Object_CleanDispose(&instanceData->triBufferSlab);
}





//=============================================================================
//      IRTriBuffer_Draw : Draw the triangle buffer.
//-----------------------------------------------------------------------------
void
IRTriBuffer_Draw(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{	TQ3Uns32		numVertices, numIndices, indicesPerVert, n, numExtra;
	TQ3FVertex3D	*theVertices;
	GLuint			*theIndices;



	// Do nothing if the buffer is empty
	if (!instanceData->triBufferActive)
		{
		Q3_ASSERT(Q3SlabMemory_GetCount(instanceData->triBufferSlab) == 0);
		return;
		}



	// Count the vertices and indices
	//
	// We have 1 triangle for every 3 vertices, and need 3 indices for each triangle.
	numVertices = Q3SlabMemory_GetCount(instanceData->triBufferSlab);
	numIndices  = numVertices;



	// Set up the triangle indices
	//
	// Rather than maintain a separate slab, we simply grow the vertex slab and stash
	// the vertices for the triangles at the end.
	//
	// Since the indices we need for each triangle are smaller than a vertex, we can
	// calculate how many indices would fit into each vertex to find out how many extra
	// vertices we need to grow the slab by to give us enough space to hold them all.
	indicesPerVert = sizeof(TQ3FVertex3D) / sizeof(GLuint);
	numExtra       = (numIndices / indicesPerVert) + 1;

	theIndices = (GLuint *) Q3SlabMemory_AppendData(instanceData->triBufferSlab, numExtra, NULL);
	if (theIndices == NULL)
		return;
	
	for (n = 0; n < numIndices; n++)
		theIndices[n] = n;



	// Get the vertices
	//
	// The vertex pointer must be fetched after the slab is grown to hold the indices,
	// since the slab may have had to be moved in order to grow.
	theVertices = (TQ3FVertex3D *) Q3SlabMemory_GetData(instanceData->triBufferSlab, 0);



	// Draw the triangles
	glVertexPointer(3, GL_FLOAT, sizeof(TQ3FVertex3D), &theVertices->thePoint);

	if (E3Bit_IsSet(instanceData->triBufferFlags, kQ3FVertexHaveNormal))
		{
		GLUtils_UpdateClientState( kQ3True, &instanceData->glClientStateNormal, GL_NORMAL_ARRAY );
		glNormalPointer(GL_FLOAT, sizeof(TQ3FVertex3D), &theVertices->theNormal);
		}
	else
		GLUtils_UpdateClientState( kQ3False, &instanceData->glClientStateNormal, GL_NORMAL_ARRAY );

	if (E3Bit_IsSet(instanceData->triBufferFlags, kQ3FVertexHaveUV))
		{
		GLUtils_UpdateClientState( kQ3True, &instanceData->glClientStateUV, GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, sizeof(TQ3FVertex3D), &theVertices->theUV);
		}
	else
		GLUtils_UpdateClientState( kQ3False, &instanceData->glClientStateUV, GL_TEXTURE_COORD_ARRAY );

	if (E3Bit_IsSet(instanceData->triBufferFlags, kQ3FVertexHaveDiffuse))
		{
		GLUtils_UpdateClientState( kQ3True, &instanceData->glClientStateColor, GL_COLOR_ARRAY );
		glColorPointer(3, GL_FLOAT, sizeof(TQ3FVertex3D), &theVertices->colourDiffuse);
		}
	else
		GLUtils_UpdateClientState( kQ3False, &instanceData->glClientStateColor, GL_COLOR_ARRAY );


	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, theIndices);



	// Update our state
	instanceData->triBufferActive = kQ3False;
	instanceData->triBufferFlags  = kQ3FVertexFlagNone;

	Q3SlabMemory_SetCount(instanceData->triBufferSlab, 0);
}





//=============================================================================
//      IRTriBuffer_AddTriangle : Add a triangle to the buffer.
//-----------------------------------------------------------------------------
TQ3Status
IRTriBuffer_AddTriangle(TQ3ViewObject			theView,
						TQ3InteractiveData		*instanceData,
						const TQ3FVertex3D		*theVertices)
{


	// Validate our parameters
	//
	// All three vertices must share the same flags, and should be opaque.
	Q3_ASSERT(theVertices[0].theFlags == theVertices[1].theFlags);
	Q3_ASSERT(theVertices[0].theFlags == theVertices[2].theFlags);
	Q3_ASSERT(E3Bit_IsNotSet(theVertices[0].theFlags, kQ3FVertexHaveTransparency));
	Q3_ASSERT(E3Bit_IsNotSet(theVertices[1].theFlags, kQ3FVertexHaveTransparency));
	Q3_ASSERT(E3Bit_IsNotSet(theVertices[2].theFlags, kQ3FVertexHaveTransparency));



	// Flush the buffer if the vertex format has changed
	if (theVertices[0].theFlags != instanceData->triBufferFlags)
		{
		IRTriBuffer_Draw(theView, instanceData);
		instanceData->triBufferFlags = theVertices[0].theFlags;
		}



	// Append the vertices to the buffer
	if (Q3SlabMemory_AppendData(instanceData->triBufferSlab, 3, theVertices) == NULL)
		return(kQ3Failure);



	// Update our state
	instanceData->triBufferActive = kQ3True;
		
	return(kQ3Success);
}

