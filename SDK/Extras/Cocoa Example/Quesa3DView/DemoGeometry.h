/*  NAME:
        DemoGeometry.h

    DESCRIPTION:
        Header file for DemoGeometry.m.

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
#import <Foundation/Foundation.h>

#include <Quesa/QuesaGeometry.h> 


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

