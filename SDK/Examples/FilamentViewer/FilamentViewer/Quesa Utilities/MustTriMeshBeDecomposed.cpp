//
//  MustTriMeshBeDecomposed.cpp
//  FilamentViewer
//
//  Created by James Walker on 6/4/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   ___________________________________________________________________________
 */

#include "MustTriMeshBeDecomposed.h"

#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/QuesaGeometry.h>


/*!
	@function	MustTriMeshBeDecomposed
	
	@abstract	Determine whether we need to decompose a TriMesh into Triangles in order to
				render it with Filament.
	
	@discussion	Currently,, our techniques for rendering Quesa objects using Filament cannot handle
				a TriMesh that does not have a texture and that does have per-face colors, so in such
				cases we fall back on decomposing the TriMesh into Triangles.
	
	@param		inTriMesh		A TriMesh.
	@result		Whether we should decompose it.
*/
bool	MustTriMeshBeDecomposed( TQ3GeometryObject _Nonnull inTriMesh )
{
	bool mustDecompose = false;
	
	// First look for a global texture
	bool hasTexture = false;
	CQ3ObjectRef atts( CQ3Geometry_GetAttributeSet( inTriMesh ) );
	if (atts.isvalid())
	{
		CQ3ObjectRef shader( CQ3AttributeSet_GetSurfaceShader( atts.get() ) );
		if (shader.isvalid())
		{
			hasTexture = true;
		}
	}
	
	if (not hasTexture)
	{
		TQ3TriMeshData* tmData = nullptr;
		if (kQ3Success == Q3TriMesh_LockData( inTriMesh, kQ3True, &tmData ))
		{
			for (TQ3Uns32 i = 0;
				(i < tmData->numTriangleAttributeTypes) and (not mustDecompose);
				++i)
			{
				const TQ3TriMeshAttributeData& oneAtt( tmData->triangleAttributeTypes[i] );
				switch (oneAtt.attributeType)
				{
					case kQ3AttributeTypeDiffuseColor:
					case kQ3AttributeTypeSpecularColor:
					case kQ3AttributeTypeTransparencyColor:
					case kQ3AttributeTypeHighlightState:
					case kQ3AttributeTypeSurfaceShader:
					case kQ3AttributeTypeEmissiveColor:
					case kQ3AttributeTypeMetallic:
						mustDecompose = true;
						break;
					
					default:
						break;
				}
			}
			Q3TriMesh_UnlockData( inTriMesh );
		}
	}
	
	return mustDecompose;
}
