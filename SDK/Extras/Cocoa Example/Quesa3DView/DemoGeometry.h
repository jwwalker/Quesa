/*  NAME:
        DemoGeometry.h

    DESCRIPTION:
        Header file for DemoGeometry.m.

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
#import <Foundation/Foundation.h>

#include <QuesaCocoa/QuesaGeometry.h> 


TQ3GeometryObject createGeomBox(void);
TQ3GeometryObject createGeomCone(void);
TQ3GeometryObject createGeomCylinder(void);
TQ3GeometryObject createGeomDisk(void);
TQ3GeometryObject createGeomEllipse(void);
TQ3GeometryObject createGeomEllipsoid(void);
TQ3GeometryObject createGeomGeneralPolygon(void);
TQ3GeometryObject createGeomLine(void);
TQ3GeometryObject createGeomMarker(void);
TQ3GeometryObject createGeomMesh(void);
TQ3GeometryObject createGeomNURBCurve(void);
TQ3GeometryObject createGeomNURBPatch(void);
TQ3GeometryObject createGeomPixmapMarker(void);
TQ3GeometryObject createGeomPoint(void);
TQ3GeometryObject createGeomPolygon(void);
TQ3GeometryObject createGeomPolyhedron(void);
TQ3GeometryObject createGeomPolyLine(void);
TQ3GroupObject createGeomQuesa(void);
TQ3GeometryObject createGeomTorus(void);
TQ3GeometryObject createGeomTriangle(void);
TQ3GeometryObject createGeomTriGrid(void);
TQ3GeometryObject createGeomTriMesh(void);

TQ3GroupObject createGeomBounds(TQ3GeometryObject theGeom);

