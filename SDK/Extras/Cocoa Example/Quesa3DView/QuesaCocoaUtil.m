/*  NAME:
        QuesaCocoaUtil.m

    DESCRIPTION:
        Utility methods.

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
#import "QuesaCocoaUtil.h"
#include <QuesaCocoa/Quesa.h>
#include <QuesaCocoa/QuesaCamera.h>
#include <QuesaCocoa/QuesaController.h>
#include <QuesaCocoa/QuesaCustomElements.h>
#include <QuesaCocoa/QuesaDrawContext.h>
#include <QuesaCocoa/QuesaErrors.h>
#include <QuesaCocoa/QuesaExtension.h>
#include <QuesaCocoa/QuesaGeometry.h>
#include <QuesaCocoa/QuesaGroup.h>
#include <QuesaCocoa/QuesaIO.h>
#include <QuesaCocoa/QuesaLight.h>
#include <QuesaCocoa/QuesaMath.h>
#include <QuesaCocoa/QuesaPick.h>
#include <QuesaCocoa/QuesaRenderer.h>
#include <QuesaCocoa/QuesaSet.h>
#include <QuesaCocoa/QuesaShader.h>
#include <QuesaCocoa/QuesaStorage.h>
#include <QuesaCocoa/QuesaString.h>
#include <QuesaCocoa/QuesaStyle.h>
#include <QuesaCocoa/QuesaTransform.h>
#include <QuesaCocoa/QuesaView.h>
#include <QuesaCocoa/QuesaViewer.h>


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
