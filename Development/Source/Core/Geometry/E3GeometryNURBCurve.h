/*  NAME:
        E3GeometryNURBCurve.h

    DESCRIPTION:
        Header file for E3GeometryNURBCurve.c.

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
#ifndef E3GEOMETRY_NURBCURVE_HDR
#define E3GEOMETRY_NURBCURVE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3GeometryNURBCurve_RegisterClass(void);
TQ3Status			E3GeometryNURBCurve_UnregisterClass(void);

TQ3GeometryObject	E3NURBCurve_New(const TQ3NURBCurveData *curveData);
TQ3Status			E3NURBCurve_Submit(const TQ3NURBCurveData *curveData, TQ3ViewObject theView);
TQ3Status			E3NURBCurve_SetData(TQ3GeometryObject theCurve, const TQ3NURBCurveData *nurbCurveData);
TQ3Status			E3NURBCurve_GetData(TQ3GeometryObject theCurve, TQ3NURBCurveData *nurbCurveData);
TQ3Status			E3NURBCurve_EmptyData(TQ3NURBCurveData *nurbCurveData);
TQ3Status			E3NURBCurve_SetControlPoint(TQ3GeometryObject theCurve, TQ3Uns32 pointIndex, const TQ3RationalPoint4D *point4D);
TQ3Status			E3NURBCurve_GetControlPoint(TQ3GeometryObject theCurve, TQ3Uns32 pointIndex, TQ3RationalPoint4D *point4D);
TQ3Status			E3NURBCurve_SetKnot(TQ3GeometryObject theCurve, TQ3Uns32 knotIndex, float knotValue);
TQ3Status			E3NURBCurve_GetKnot(TQ3GeometryObject theCurve, TQ3Uns32 knotIndex, float *knotValue);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

