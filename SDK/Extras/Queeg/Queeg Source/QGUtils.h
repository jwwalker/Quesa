//Header File for QGUtils.cpp

//for descriptions of functions see QGUtils.cpp

#ifndef QGUTILS_H
#define QGUTILS_H



#ifndef __QD3D__
#include <QD3D.h>
#endif




TQ3Boolean QG_LineIntersectsFacet(const TQ3Point3D& p1,const TQ3Point3D& p2, // line segment
									 const TQ3Point3D& pa, const TQ3Point3D& pb, const TQ3Point3D& pc, // facet
									 TQ3Point3D *outpoint);

TQ3Boolean QG_LineIntersectsBox(const TQ3Point3D& p1,const TQ3Point3D& p2, // line segment
									const TQ3BoundingBox& bbox,		// axis-aligned bounding box
									TQ3Point3D *outpoint=NULL);

//utility routines

void QGPoint3D_Normalize(const TQ3Point3D& a,TQ3Point3D *outpoint);



#endif