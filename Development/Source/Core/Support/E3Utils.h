/*  NAME:
        E3Utils.h

    DESCRIPTION:
        Header file for E3Utils.c.

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
#ifndef E3UTILS_HDR
#define E3UTILS_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef TQ3AttributeSet (*E3GetSetForGatherProc)(const void *userData, TQ3Uns32 setIndex);





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define E3Rect_GetWidth(_r)					(TQ3Uns32) ((_r)->right  - (_r)->left)

#define E3Rect_GetHeight(_r)				(TQ3Uns32) ((_r)->bottom - (_r)->top)

#define E3Area_FromRect(_area, _rect)											\
				do																\
					{															\
					(_area)->min.x = (float) (_rect)->left;						\
					(_area)->min.y = (float) (_rect)->top;						\
					(_area)->max.x = (float) (_rect)->right;					\
					(_area)->max.y = (float) (_rect)->bottom;					\
					}															\
				while (0)

#define E3Area_ToRect(_area, _rect)												\
				do																\
					{															\
					(_rect)->left   = (TQ3Int16) (_area)->min.x;				\
					(_rect)->top    = (TQ3Int16) (_area)->min.y;				\
					(_rect)->right  = (TQ3Int16) (_area)->max.x;				\
					(_rect)->bottom = (TQ3Int16) (_area)->max.y;				\
					}															\
				while (0)

#define E3Integer_Swap(_a, _b)													\
				do																\
					{															\
					TQ3Uns32 _temp;												\
																				\
					_temp = (_a);												\
					(_a)  = (_b);												\
					(_b)  = _temp;												\
					}															\
				while (0)

#define E3Float_Swap(_a, _b)													\
				do																\
					{															\
					float _temp;												\
																				\
					_temp = (_a);												\
					(_a)  = (_b);												\
					(_b)  = _temp;												\
					}															\
				while (0)

#define E3Num_Min(_a, _b)					((_a) < (_b) ? (_a) : (_b))

#define E3Num_Max(_a, _b)					((_a) > (_b) ? (_a) : (_b))

#define E3Integer_Abs(_a)					((_a) > 0 ? (_a) : -(_a))

#define E3Float_Abs(_a)						((_a) > 0.0f ? (_a) : -(_a))

#define E3Bit_Set(_bf, _b)					(_bf) |= (_b)

#define E3Bit_Clear(_bf, _b)				(_bf) &= ~(_b)

#define E3Bit_AnySet(_bf, _b)				((TQ3Boolean) (((_bf) & (_b)) != 0))

#define E3Bit_IsSet(_bf, _b)				((TQ3Boolean) (((_bf) & (_b)) == (_b)))

#define E3Bit_IsNotSet(_bf, _b)				!E3Bit_IsSet(_bf, _b)

#define E3EndianSwap16(_value)                                        	\
                        (((((TQ3Uns16) _value) << 8) & 0xFF00)       |	\
                         ((((TQ3Uns16) _value) >> 8) & 0x00FF))
    
#define E3EndianSwap32(_value)                                      	\
                        (((((TQ3Uns32) _value) << 24) & 0xFF000000)  |	\
                         ((((TQ3Uns32) _value) <<  8) & 0x00FF0000)  |	\
                         ((((TQ3Uns32) _value) >>  8) & 0x0000FF00)  |	\
                         ((((TQ3Uns32) _value) >> 24) & 0x000000FF))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
void		E3Shared_Acquire(TQ3SharedObject *newRef, TQ3SharedObject theObject);

void		E3Shared_Replace(TQ3SharedObject *origObject, TQ3SharedObject newObject);

TQ3Status	E3Bitmap_Replace(const TQ3Bitmap *original, TQ3Bitmap *copy, TQ3Boolean isCopyInternal);

void		E3AttributeSet_Combine(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet *result);

void		E3Geometry_AddNormalIndicators(TQ3GroupObject group, TQ3Uns32 numPoints, const TQ3Point3D *points, const TQ3Vector3D *normals);

TQ3Boolean	E3Matrix4x4_IsIdentity(const TQ3Matrix4x4 *theMatrix);

TQ3Boolean	E3CString_IsEqual(const char *str_a, const char *str_b);

TQ3Boolean	E3Rect_ClipLine(const TQ3Area *theRect, TQ3Point2D *lineStart, TQ3Point2D *lineEnd);

TQ3Boolean	E3Rect_ContainsLine(const TQ3Area *theRect, const TQ3Point2D *lineStart, const TQ3Point2D *lineEnd);

TQ3Boolean	E3Rect_ContainsRect(const TQ3Area *rect1, const TQ3Area *rect2);

TQ3Boolean	E3Rect_IntersectRect(const TQ3Area *rect1, const TQ3Area *rect2);

void		E3Triangle_InterpolateHit(const TQ3TriangleData		*triangleData,
										const TQ3Param3D		*theHit,
										TQ3Point3D				*hitXYZ,
										TQ3Vector3D				*hitNormal,
										TQ3Param2D				*hitUV,
										TQ3Boolean				*haveUV);

TQ3Boolean	E3TriMeshAttribute_GatherArray(TQ3Uns32						numSets,
											E3GetSetForGatherProc		userCallback,
											const void					*userData,
											TQ3TriMeshAttributeData		*theAttribute,
											TQ3AttributeType			attributeType);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

