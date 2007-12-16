/*  NAME:
        QuesaCocoaUtil.m

    DESCRIPTION:
        Utility methods.

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
#import "QuesaCocoaUtil.h"
#include <Quesa/Quesa.h>
#include <Quesa/QuesaCamera.h>
#include <Quesa/QuesaController.h>
#include <Quesa/QuesaCustomElements.h>
#include <Quesa/QuesaDrawContext.h>
#include <Quesa/QuesaErrors.h>
#include <Quesa/QuesaExtension.h>
#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaGroup.h>
#include <Quesa/QuesaIO.h>
#include <Quesa/QuesaLight.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaPick.h>
#include <Quesa/QuesaRenderer.h>
#include <Quesa/QuesaSet.h>
#include <Quesa/QuesaShader.h>
#include <Quesa/QuesaStorage.h>
#include <Quesa/QuesaString.h>
#include <Quesa/QuesaStyle.h>
#include <Quesa/QuesaTransform.h>
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaViewer.h>


void boundsForObject(TQ3ViewObject theView,TQ3Object theObject, TQ3BoundingBox* theBounds)
{	
    float			sizeX, sizeY, sizeZ;
	TQ3Status		qd3dStatus;


	// Reset theBounds
	memset(theBounds, 0x00, sizeof(TQ3BoundingBox));
	


	// Submit the object to the view to calculate the bounds
	qd3dStatus = Q3View_StartBoundingBox(theView, kQ3ComputeBoundsExact);
	if (qd3dStatus == kQ3Success)
    {
		do
        {
			Q3Object_Submit(theObject, theView);
        }
		while (Q3View_EndBoundingBox(theView, theBounds) == kQ3ViewStatusRetraverse);
    }



	// If we have an empty bounding box, bump it up slightly
	sizeX = theBounds->max.x - theBounds->min.x;
	sizeY = theBounds->max.y - theBounds->min.y;
	sizeZ = theBounds->max.z - theBounds->min.z;

	if (sizeX <= kQ3RealZero && sizeY <= kQ3RealZero && sizeZ <= kQ3RealZero)
    {
		theBounds->max.x += 0.0001f;
		theBounds->max.y += 0.0001f;
		theBounds->max.z += 0.0001f;
			
		theBounds->min.x -= 0.0001f;
		theBounds->min.y -= 0.0001f;
		theBounds->min.z -= 0.0001f;
    }
}
