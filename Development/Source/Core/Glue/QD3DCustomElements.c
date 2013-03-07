/*  NAME:
        QD3DCustomElements.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2013, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      CENameElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
CENameElement_SetData(TQ3Object object, const char *name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_SetData(object, name));
}





//=============================================================================
//      CENameElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CENameElement_GetData(TQ3Object object, char **name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_GetData(object, name));
}





//=============================================================================
//      CENameElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CENameElement_PeekData(TQ3Object object, const char **name)
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), kQ3Failure);
	


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return (E3NameElement_PeekData(object, name));
}





//=============================================================================
//      CENameElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CENameElement_EmptyData(char **name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_EmptyData(name));
}




#pragma mark -
//=============================================================================
//      CEUrlElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_SetData(object, urlData));
}





//=============================================================================
//      CEUrlElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_GetData(TQ3Object object, TCEUrlData **urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object),  kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(urlData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_GetData(object, urlData));
}





//=============================================================================
//      CEUrlElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_EmptyData(TCEUrlData **urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(urlData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_EmptyData(urlData));
}



#pragma mark -

//=============================================================================
//      CEWireElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
TQ3Status
CEWireElement_SetData(TQ3Object object, QTAtomContainer wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_SetData(object, wireData));
}





//=============================================================================
//      CEWireElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEWireElement_GetData(TQ3Object object, QTAtomContainer *wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object),   kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wireData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_GetData(object, wireData));
}





//=============================================================================
//      CEWireElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEWireElement_EmptyData(QTAtomContainer *wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wireData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_EmptyData(wireData));
}
#endif



#pragma mark -

/*!
	@function	CETriangleStripElement_SetData
	@abstract	Set a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.  If you have not
				already provided a triangle strip for a TriMesh, the renderer
				will compute one, but this can take a little time.
				
				When you assign a triangle strip, the element also records the
				current edit index of the object.
				
				You can pass 0 for inNumIndices and NULL for inIndices to
				indicate that you want to avoid using a triangle strip, perhaps
				because there is no efficient strip for this geometry.
				
				<em>This function is not available in QD3D.</em>
	@param		ioObject		An object, normally a TriMesh.
	@param		inNumIndices	Count of indices in the following array.
	@param		inIndices		Array of vertex indices making the strip.
	@result		Success or failure of the operation.
*/
TQ3Status
CETriangleStripElement_SetData(
	TQ3Object ioObject,
	TQ3Uns32 inNumIndices,
	const TQ3Uns32* inIndices
)
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ioObject), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(ioObject->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( ioObject ), kQ3Failure);
	if (inNumIndices > 0)
	{
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inIndices), kQ3Failure);
	}


	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return E3TriangleStripElement_SetData( ioObject, inNumIndices, inIndices );
}

/*!
	@function	CETriangleStripElement_GetData
	@abstract	Get a triangle strip for the object.
	@discussion	Triangle strips are used by the OpenGL renderer as a speed
				optimization for rendering TriMesh objects.
				
				If the current edit index of the object is not the same as
				when a strip was assigned, the strip will be considered stale
				and this function will return kQ3Failure.
				
				This function returns the actual triangle strip data within
				the element, not a copy.  The data should be considered
				read-only and temporary.
				
				<em>This function is not available in QD3D.</em>
	@param		inObject		An object, normally a TriMesh.
	@param		outNumIndices	Receives count of indices.
	@param		outIndices		Receives address of array of vertex indices.
	@result		Success or failure of the operation.
*/
TQ3Status
CETriangleStripElement_GetData(
	TQ3Object inObject,
	TQ3Uns32* outNumIndices,
	const TQ3Uns32** outIndices
)
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inObject), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outNumIndices), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outIndices), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(inObject->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( inObject ), kQ3Failure);


	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return E3TriangleStripElement_GetData( inObject, outNumIndices, outIndices );
}



/*!
	@function	CETriangleStripElement_Remove
	@abstract	Remove a triangle strip element if it exists.
	@param		ioObject		An object, normally a TriMesh.
*/
void
CETriangleStripElement_Remove(
	TQ3Object ioObject
)
{
	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(ioObject));
	Q3_REQUIRE( ioObject->IsObjectValid () );
	Q3_REQUIRE( E3Shared_IsOfMyClass ( ioObject ) );
	

	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3TriangleStripElement_Remove( ioObject );
}

#pragma mark -

/*!
	@function	CESpecularMapElement_Copy
	@abstract	Retrieve a specular map texture from an object.
	@param		inShader		An object, normally a surface shader.
	@result		A new reference to a texture, or NULL.
*/
TQ3TextureObject
CESpecularMapElement_Copy( TQ3ShaderObject inShader )
{
	Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR(inShader), NULL );
	Q3_REQUIRE_OR_RESULT( inShader->IsObjectValid(), NULL );
	
	// Call the bottleneck
	E3System_Bottleneck();
	
	TQ3TextureObject theTexture = E3SpecularMapElement_Copy( inShader );
	return theTexture;
}



/*!
	@function	CESpecularMapElement_Set
	@abstract	Set or remove a specular map.
	@discussion	The specular map texture must be an RGBA texture.
				The RGB part is the specular color, while the alpha
				channel serves as specular control.  The alpha is not
				the same as Quesa's usual specular control, since
				specular control is an unbounded floating-point number,
				while the alpha value is bounded. To be exact,
				an alpha value in the range [0.0, 1.0] is multiplied
				by 128.0 and used as the OpenGL specular exponent.
				
				A specular map may only be used with the OpenGL
				renderer, and requires that per-pixel lighting be
				enabled.
	@param		ioShader	A surface shader.
	@param		inTexture	A texture object, or NULL to remove.
*/
void
CESpecularMapElement_Set( TQ3ShaderObject ioShader, TQ3TextureObject inTexture )
{
	Q3_REQUIRE(Q3_VALID_PTR(ioShader));
	Q3_REQUIRE( ioShader->IsObjectValid () );
	Q3_REQUIRE( (inTexture == NULL) || inTexture->IsObjectValid () );
	
	// Call the bottleneck
	E3System_Bottleneck();
	
	E3SpecularMapElement_Set( ioShader, inTexture );
}

