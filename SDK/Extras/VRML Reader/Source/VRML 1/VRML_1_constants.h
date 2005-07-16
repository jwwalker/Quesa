#pragma once
/*  NAME:
        VRML_1_constants.h

    DESCRIPTION:
        Constants used in VRML 1.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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

/*!
	@header		VRML 1 constants
	
	VRML version 1 uses various enumerated constants and bitmasks.
	Here I list the string values and provide numeric values for them.
	
	Note: The grammar will probably not distinguish between kEnumMap_NONE
	and kMaskStyle_NONE, so I have been careful to give eVRML1Style_NONE
	and eVRML1Map_NONE the same value.
*/

extern const char*	kEnumJustification_LEFT;
extern const char*	kEnumJustification_CENTER;
extern const char*	kEnumJustification_RIGHT;

extern const char*	kEnumFamily_SERIF;
extern const char*	kEnumFamily_SANS;
extern const char*	kEnumFamily_TYPEWRITER;

extern const char*	kEnumValue_DEFAULT;
extern const char*	kEnumValue_OVERALL;
extern const char*	kEnumValue_PER_PART;
extern const char*	kEnumValue_PER_PART_INDEXED;
extern const char*	kEnumValue_PER_FACE;
extern const char*	kEnumValue_PER_FACE_INDEXED;
extern const char*	kEnumValue_PER_VERTEX;
extern const char*	kEnumValue_PER_VERTEX_INDEXED;

extern const char*	kEnumCulling_ON;
extern const char*	kEnumCulling_OFF;
extern const char*	kEnumCulling_AUTO;

extern const char*	kEnumVertexOrdering_UNKNOWN_ORDERING;
extern const char*	kEnumVertexOrdering_COUNTERCLOCKWISE;
extern const char*	kEnumVertexOrdering_CLOCKWISE;

extern const char*	kEnumShapeType_UNKNOWN_SHAPE_TYPE;
extern const char*	kEnumShapeType_SOLID;

extern const char*	kEnumFaceType_UNKNOWN_FACE_TYPE;
extern const char*	kEnumFaceType_CONVEX;

extern const char*	kEnumWrap_REPEAT;
extern const char*	kEnumWrap_CLAMP;

extern const char*	kEnumMap_NONE;
extern const char*	kEnumMap_POINT;

extern const char*	kMaskParts_SIDES;
extern const char*	kMaskParts_TOP;
extern const char*	kMaskParts_BOTTOM;
extern const char*	kMaskParts_ALL;

extern const char*	kMaskStyle_NONE;
extern const char*	kMaskStyle_BOLD;
extern const char*	kMaskStyle_ITALIC;


enum EVRML1Value
{
	eVRML1Parts_SIDES						= 1,
	eVRML1Parts_TOP							= 2,
	eVRML1Parts_BOTTOM						= 4,
	eVRML1Parts_ALL							= 7,
	
	eVRML1Style_NONE						= 0,
	eVRML1Style_BOLD						= 1,
	eVRML1Style_ITALIC						= 2,
	
	eVRML1Justification_LEFT				= 1,
	eVRML1Justification_CENTER				= 2,
	eVRML1Justification_RIGHT				= 3,
	
	eVRML1Family_SERIF						= 1,
	eVRML1Family_SANS						= 2,
	eVRML1Family_TYPEWRITER					= 3,

	eVRML1Value_DEFAULT						= 0,
	eVRML1Value_OVERALL						= 1,
	eVRML1Value_PER_PART					= 2,
	eVRML1Value_PER_PART_INDEXED			= 3,
	eVRML1Value_PER_FACE					= 4,
	eVRML1Value_PER_FACE_INDEXED			= 5,
	eVRML1Value_PER_VERTEX					= 6,
	eVRML1Value_PER_VERTEX_INDEXED			= 7,

	eVRML1Culling_OFF						= 0,
	eVRML1Culling_ON						= 1,
	eVRML1Culling_AUTO						= 2,

	eVRML1VertexOrdering_UNKNOWN_ORDERING	= 0,
	eVRML1VertexOrdering_COUNTERCLOCKWISE	= 1,
	eVRML1VertexOrdering_CLOCKWISE			= 2,

	eVRML1ShapeType_UNKNOWN_SHAPE_TYPE		= 0,
	eVRML1ShapeType_SOLID					= 1,

	eVRML1FaceType_UNKNOWN_FACE_TYPE		= 0,
	eVRML1FaceType_CONVEX					= 1,

	eVRML1Wrap_REPEAT						= 1,
	eVRML1Wrap_CLAMP						= 2,

	eVRML1Map_NONE							= 0,
	eVRML1Map_POINT							= 1
};
