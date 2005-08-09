/*  NAME:
        MakeTransform.cp

    DESCRIPTION:
       Turn a Transform node into a Quesa transform object.

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
#include "MakeTransform.h"

#include "IsKeyPresent.h"

#if __MACH__
	#include <Quesa/QuesaMath.h>
	#include <Quesa/QuesaTransform.h>
#else
	#include <QuesaMath.h>
	#include <QuesaTransform.h>
#endif


static bool	GetVec3Value( PolyValue::Dictionary& inDict, const char* inName,
							TQ3Vector3D& outVec )
{
	bool	gotVec = false;
	
	if (IsKeyPresent( inDict, inName ))
	{
		PolyValue&	theValue( inDict[ inName ] );
		if (theValue.IsNumberVec())
		{
			PolyValue::FloatVec&	theFloatVec( theValue.GetFloatVec() );
			if (theFloatVec.size() == 3)
			{
				outVec.x = theFloatVec[0];
				outVec.y = theFloatVec[1];
				outVec.z = theFloatVec[2];
				gotVec = true;
			}
		}
	}
	return gotVec;
}

static bool GetRotationValue( PolyValue::Dictionary& inDict, const char* inName,
								TQ3Matrix4x4& outMatrix )
{
	bool	gotVec = false;
	
	if (IsKeyPresent( inDict, inName ))
	{
		PolyValue&	theValue( inDict[ inName ] );
		if (theValue.IsNumberVec())
		{
			PolyValue::FloatVec&	theFloatVec( theValue.GetFloatVec() );
			if (theFloatVec.size() == 4)
			{
				TQ3Vector3D	theAxis;
				theAxis.x = theFloatVec[0];
				theAxis.y = theFloatVec[1];
				theAxis.z = theFloatVec[2];
				float	theAngle = theFloatVec[3];
				TQ3Point3D	theOrigin = { 0.0f, 0.0f, 0.0f };
				// VRML specs say that the axis will be normalized, but I
				// do not trust VRML writers to do that.
				float	axisLen = Q3FastVector3D_Length( &theAxis );
				if (axisLen > FLT_EPSILON)
				{
					Q3FastVector3D_Scale( &theAxis, 1.0f/axisLen, &theAxis );
				}
				Q3Matrix4x4_SetRotateAboutAxis( &outMatrix, &theOrigin,
					&theAxis, theAngle );
				gotVec = true;
			}
		}
	}
	return gotVec;
}


/*!
	@function	MakeTransform
	
	@abstract	Create a Quesa matrix transform object from information in
				a VRML 1 or 2 Transform node.
	
	@param		inTransformDict		Field dictionary of the node.
	
	@result		A transform object.
*/
CQ3ObjectRef	MakeTransform( PolyValue::Dictionary& inTransformDict )
{
	CQ3ObjectRef	theTransform;
	TQ3Matrix4x4	theMatrix;
	Q3Matrix4x4_SetIdentity( &theMatrix );
	bool	haveTransform = false;
	TQ3Vector3D	scaleVec;
	
	if (GetVec3Value( inTransformDict, "scale", scaleVec ))
	{
		Q3Matrix4x4_SetScale( &theMatrix, scaleVec.x, scaleVec.y, scaleVec.z );
		haveTransform = true;
		TQ3Matrix4x4	scaleOrient;
		
		if (GetRotationValue( inTransformDict, "scaleOrientation", scaleOrient ))
		{
			TQ3Matrix4x4	orientInv;
			Q3Matrix4x4_Invert( &scaleOrient, &orientInv );
			Q3Matrix4x4_Multiply( &orientInv, &theMatrix, &theMatrix );
			Q3Matrix4x4_Multiply( &theMatrix, &scaleOrient, &theMatrix );
		}
	}
	
	TQ3Matrix4x4	rotMtx;
	if (GetRotationValue( inTransformDict, "rotation", rotMtx ))
	{
		Q3Matrix4x4_Multiply( &theMatrix, &rotMtx, &theMatrix );
		haveTransform = true;
	}
	
	TQ3Vector3D		centerVec;
	if ( haveTransform and GetVec3Value( inTransformDict, "center", centerVec ) )
	{
		TQ3Matrix4x4	originToCenter, centerToOrigin;
		Q3Matrix4x4_SetTranslate( &originToCenter, centerVec.x, centerVec.y, centerVec.z );
		Q3Matrix4x4_SetTranslate( &centerToOrigin, -centerVec.x, -centerVec.y, -centerVec.z );
		Q3Matrix4x4_Multiply( &centerToOrigin, &theMatrix, &theMatrix );
		Q3Matrix4x4_Multiply( &theMatrix, &originToCenter, &theMatrix );
	}
	
	TQ3Vector3D		translateVec;
	if (GetVec3Value( inTransformDict, "translation", translateVec ))
	{
		TQ3Matrix4x4	translateMtx;
		Q3Matrix4x4_SetTranslate( &translateMtx, translateVec.x, translateVec.y, translateVec.z );
		Q3Matrix4x4_Multiply( &theMatrix, &translateMtx, &theMatrix );
		haveTransform = true;
	}
	
	if (haveTransform)
	{
		theTransform = CQ3ObjectRef( Q3MatrixTransform_New( &theMatrix ) );
	}
	
	return theTransform;
}
