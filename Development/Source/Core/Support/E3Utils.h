/*  NAME:
        E3Utils.h

    DESCRIPTION:
        Header file for E3Utils.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#ifndef E3UTILS_HDR
#define E3UTILS_HDR





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef TQ3AttributeSet (*E3GetSetForGatherProc)(const void *userData, TQ3Uns32 setIndex);





//=============================================================================
//      Macros and template
//-----------------------------------------------------------------------------

#ifdef __cplusplus

template<typename T> inline
const T& E3Num_Max(const T& a, const T& b)
{
	return ( (a < b) ? b : a);
}

template<typename T> inline
const T& E3Num_Min(const T& a, const T& b)
{
	return ( (a < b) ? a : b);
}

template <typename T, typename Tmin, typename Tmax> inline
T E3Num_Clamp( const T& _n, const Tmin& _min, const Tmax& _max )
{
	return E3Num_Max( E3Num_Min( _n, static_cast<T>(_max) ), static_cast<T>(_min) );
}

#else // pure C

#define E3Num_Min(_a, _b)					((_a) < (_b) ? (_a) : (_b))

#define E3Num_Max(_a, _b)					((_a) > (_b) ? (_a) : (_b))

#define E3Num_Clamp(_n, _min, _max)			E3Num_Max(E3Num_Min((_n), (_max)), (_min))

#endif


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



#define E3Integer_Abs(_a)					((_a) > 0 ? (_a) : -(_a))

#define E3Float_Abs(_a)						((_a) > 0.0f ? (_a) : -(_a))

#define E3Bit_Set(_bf, _b)					(_bf) |= (_b)

#define E3Bit_Clear(_bf, _b)				(_bf) &= ~(_b)

#define E3Bit_AnySet(_bf, _b)				((TQ3Boolean) (((_bf) & (_b)) != 0))

#define E3Bit_IsSet(_bf, _b)				((TQ3Boolean) (((_bf) & (_b)) == (_b)))

#define E3Bit_IsNotSet(_bf, _b)				((TQ3Boolean) !E3Bit_IsSet(_bf, _b))

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

void		E3Triangle_InterpolateHit (	TQ3ViewObject			theView,
										const TQ3TriangleData	*triangleData,
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

TQ3GroupObject	E3TriMesh_BuildOrientationGroup(TQ3GeometryObject theTriMesh, TQ3OrientationStyle theOrientation);





#endif
