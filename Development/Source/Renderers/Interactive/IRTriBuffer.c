/*  NAME:
        IRTriBuffer.c

    DESCRIPTION:
        Quesa interactive renderer triangle buffer.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

