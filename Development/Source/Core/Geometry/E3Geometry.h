/*  NAME:
        E3Geometry.h

    DESCRIPTION:
        Header file for E3Geometry.c.

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
#ifndef E3GEOMETRY_HDR
#define E3GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3Main.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Geometry data
class E3Geometry : public E3ShapeData
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeGeometry, E3Geometry, E3ShapeData )
	TQ3Uns32					cameraEditIndex;
	TQ3SubdivisionStyleData		styleSubdivision;
	TQ3OrientationStyle			styleOrientation;
	TQ3Uns32					cachedEditIndex;
	TQ3Object					cachedObject;
	float						cachedDeterminant;
	
	
public :

	friend TQ3Status			e3geometry_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 								TQ3Object toObject,   void       *toPrivateData) ;
	friend void					e3geometry_delete(TQ3Object theObject, void *privateData) ;
	friend TQ3Boolean			e3geometry_cache_isvalid(TQ3ViewObject theView,
													TQ3ObjectType objectType, TQ3GeometryObject theGeom,
													const void   *geomData,   TQ3Object         cachedGeom)	;
	friend TQ3Status			e3geometry_submit_decomposed(TQ3ViewObject theView, TQ3ObjectType objectType,
													TQ3Object theObject, const void *objectData) ;
	} ;



class E3GeometryInfo : public E3ShapeInfo
	{
	const TQ3XGeomCacheIsValidMethod	cacheIsValid ;
	const TQ3XGeomCacheUpdateMethod 	cacheUpdate ;

public :

	const TQ3XGeomCacheNewMethod	 	cacheNew ;
	const TQ3XGeomGetAttributeMethod	getAttribute ;
	const TQ3XGeomGetPublicDataMethod	getPublicData ;
	
									E3GeometryInfo	(
											TQ3XMetaHandler	newClassMetaHandler,
											E3ClassInfo*	newParent
			 								) ; // constructor	

	friend TQ3Status e3geometry_submit_decomposed ( TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData) ;
	} ;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Geometry_RegisterClass(void);
TQ3Status			E3Geometry_UnregisterClass(void);

TQ3ObjectType		E3Geometry_GetType(TQ3GeometryObject theGeom);
TQ3Status			E3Geometry_GetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet *attributeSet);
TQ3Status			E3Geometry_SetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet attributeSet);
TQ3Status			E3Geometry_Submit(TQ3GeometryObject theGeom, TQ3ViewObject theView);
TQ3Object			E3Geometry_GetDecomposed( TQ3GeometryObject theGeom, TQ3ViewObject view );

TQ3Boolean			E3Geometry_IsDegenerateTriple( const TQ3Vector3D* orientation,
												const TQ3Vector3D* majorAxis,
												const TQ3Vector3D* minorAxis );


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

