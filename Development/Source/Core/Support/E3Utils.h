/*  NAME:
        E3Utils.h

    DESCRIPTION:
        Header file for E3Utils.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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

#define E3Object_DisposeAndForget(_qd3dObject)									\
				do																\
					{															\
					if ((_qd3dObject) != NULL)									\
						Q3Object_Dispose((_qd3dObject));						\
																				\
					(_qd3dObject) = NULL;										\
					}															\
				while (0)

#define E3Group_AddObjectAndDispose(_qd3dGroup, _qd3dObject)					\
				do																\
					{															\
					if ((_qd3dObject) != NULL)									\
						Q3Group_AddObject((_qd3dGroup), (_qd3dObject));			\
																				\
					E3Object_DisposeAndForget((_qd3dObject));					\
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

#define E3Num_Abs(_a)						((_a) > 0 ? (_a) : -(_a))

#define E3EndianSwap16(_value)                                        	\
                        (((((TQ3Uns16) _value) << 8) & 0xFF00)       |	\
                         ((((TQ3Uns16) _value) >> 8) & 0x00FF))
    
#define E3EndianSwap32(_value)                                      		\
                        (((((TQ3Uns32) _value) << 24) & 0xFF000000)  |	\
                         ((((TQ3Uns32) _value) <<  8) & 0x00FF0000)  |	\
                         ((((TQ3Uns32) _value) >>  8) & 0x0000FF00)  |	\
                         ((((TQ3Uns32) _value) >> 24) & 0x000000FF))

#define E3Bit_Test(_bf, _b)						((TQ3Boolean) (((_bf) & (_b)) == (_b)))

#define E3Bit_Set(_bf, _b)						(_bf) |= (_b)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
void		E3Shared_Acquire(TQ3SharedObject *newRef, TQ3SharedObject theObject);
void		E3Shared_Replace(TQ3SharedObject *origObject, TQ3SharedObject newObject);
TQ3Status	E3Bitmap_Replace(const TQ3Bitmap *original, TQ3Bitmap *copy, TQ3Boolean isCopyInternal);
void		E3AttributeSet_Combine(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet *result);
void		E3Geometry_AddNormalIndicators(TQ3GroupObject group, TQ3Uns32 numPoints, const TQ3Point3D *points, const TQ3Vector3D *normals);
TQ3Boolean	E3Matrix4x4_IsIdentity(const TQ3Matrix4x4 *theMatrix);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

