/*  NAME:
        SignedAngleBetweenVectors.cp

    DESCRIPTION:
       Math utility functon.

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
#include "SignedAngleBetweenVectors.h"

#include <QuesaMath.h>

#include <cmath>

namespace std {	// for stupid BSD headers
}

using namespace std;

/*!
	@function	SignedAngleBetweenVectors
	
	@abstract	Find the angle between two vectors.
	
	@discussion	Assuming that the first and second vectors are orthogonal
				to the axis, find the angle of rotation about the axis
				that will move the first vector to match the direction
				of the second.
				
				All three vectors are assumed to be nonzero.
	
	@param		inFirst		A vector.
	@param		inSecond	A vector.
	@param		inAxis		Axis of rotation.
	@result		Angle in radians, between -pi and pi.
*/
float	SignedAngleBetweenVectors( const TQ3Vector3D& inFirst,
							const TQ3Vector3D& inSecond,
							const TQ3Vector3D& inAxis )
{
	float	lenSq1 = Q3FastVector3D_LengthSquared( &inFirst );
	float	lenSq2 = Q3FastVector3D_LengthSquared( &inSecond );
	
	float	angleCos = Q3Vector3D_Dot( &inFirst, &inSecond ) /
		sqrt( lenSq1 * lenSq2 );
	
	// Arithmetic error might result in angleCos being slightly outside
	// the expected range [-1, 1], which would cause acos to return NAN.
	if (angleCos > 1.0f)
	{
		angleCos = 1.0f;
	}
	else if (angleCos < -1.0f)
	{
		angleCos = -1.0f;
	}
	
	float	angleRadians = acos( angleCos );
	
	if (angleRadians < kQ3RealZero)
	{
		angleRadians = 0.0f;
	}
	
	// The cosine doesnÕt tell us whether the angle is positive or negative,
	// but we can figure that out from the cross product.
	TQ3Vector3D		crossVec;
	Q3FastVector3D_Cross( &inFirst, &inSecond, &crossVec );
	if (Q3FastVector3D_Dot( &crossVec, &inAxis ) < 0.0f)
	{
		angleRadians = -angleRadians;
	}
	
	return angleRadians;
}
