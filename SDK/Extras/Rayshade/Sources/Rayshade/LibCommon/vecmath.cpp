/*
 * vecmath.c
 *
 * Copyright (C) 1989, 1991, Craig E. Kolb
 * All rights reserved.
 *
 * This software may be freely copied, modified, and redistributed
 * provided that this copyright notice is preserved on all copies.
 *
 * You may not distribute this software, in whole or in part, as part of
 * any commercial product without the express consent of the authors.
 *
 * There is no warranty or other guarantee of fitness of this software
 * for any purpose.  It is provided solely "as is".
 *
 * $Id: vecmath.cpp,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.2  91/11/26  21:36:15  cek
 * patch3: Potential roundoff problems.
 * 
 * Revision 4.0.1.1  91/09/29  15:38:41  cek
 * patch1: Fixed floating-point compare in normalization code.
 *
 * Revision 4.0  91/07/17  14:33:02  kolb
 * Initial version.
 * 
 */
#include "common.h"

/*
 * Normalize a vector, return original length.
 */
Float
VecNormalize(register Vector *a)
{
	Float d;

	d = sqrt(a->x*a->x + a->y*a->y + a->z*a->z);
	if (equal(d, 0.))
		return 0.;
	a->x /= d;
	a->y /= d;
	a->z /= d;
#ifdef CRAY
	/*
	 * The Cray Research Inc. math functional units don't work in the IEEE
	 * standard way, so when we get here, we just might have an x,y or z
	 * that is not in the range  -1.0 <= J <= 1.0 Yes, I know that that
	 * can't happen, but it does. So since we know we just normalized this
	 * vector, we'll just force x,y and z into the range -1.0 to 1.0 O.K?
	 */
	if (a->x >= 1.0) a->x = 1.0;
	else if (a->x <= -1.0) a->x = -1.0;
	if (a->y >= 1.0) a->y = 1.0;
	else if (a->y <= -1.0) a->y = -1.0;
	if (a->z >= 1.0) a->z = 1.0;
	else if (a->z <= -1.0) a->z = -1.0;
#endif /* CRAY */

	return d;
}

/*
 * Compute cross-product of a and b, place normalized result in o.  Returns
 * length of result before normalization.
 */
Float
VecNormCross(
        Vector          *a, 
        Vector          *b, 
        Vector          *r)
{
	VecCross(a, b, r);
	return VecNormalize(r);
}

/*
 * Compute cross-product of a and b, place result in o.
 */
void
VecCross(Vector *a,Vector * b,Vector * r)
{
	r->x = (a->y * b->z) - (a->z * b->y);
	r->y = (a->z * b->x) - (a->x * b->z);
	r->z = (a->x * b->y) - (a->y * b->x);
}

/*
 * Calculate direction of refracted ray using Heckbert's formula.  Returns TRUE
 * if a total internal reflection occurs.
 */
int
Refract(Vector *dir, Float from_index,Float to_index,Vector* I,Vector* N,Float cos1)
{
	double kn, cos2, k;
	Vector nrm;

	if (cos1 < 0.) {
		/*
		 * Hit the 'backside' of a surface -- flip the normal.
		 */
		nrm.x = -N->x;
		nrm.y = -N->y;
		nrm.z = -N->z;
		cos1 = -cos1;
	} else
		nrm = *N;

	kn = from_index / to_index;
	cos2 = 1. - kn*kn*(1. - cos1*cos1);
	if (cos2 < 0.)
		return TRUE;		/* Total internal reflection. */
	k = kn * cos1 - sqrt((double)cos2);
	VecComb(kn, *I, k, nrm, dir);
	return FALSE;
}

/*
 * Given a vector, find two additional vectors such that all three
 * are mutually perpendicular and uaxis X vaxis = vector.  The given
 * vector need not be normalized. uaxis and vaxis are normalized.
 */
void
VecCoordSys(Vector *vector,Vector *uaxis,Vector *vaxis)
{
	uaxis->x = -vector->y;
	uaxis->y = vector->x;
	uaxis->z = 0.;
	if (VecNormalize(uaxis) == 0.) {
		uaxis->x = vector->z;
		uaxis->y = 0.;
		uaxis->z = -vector->x;
		if (VecNormalize(uaxis) == 0.)
			RLerror(RL_WARN,
				"VecCoordSys passed degenerate vector.\n");
	}
	(void)VecNormCross(vector, uaxis, vaxis);
}

/*
 * Modify given normal by "bumping" it.
 */
void
MakeBump(
        Vector      *norm,  /* normal */
        Vector      *dpdu,  /* surface derivatives */
        Vector      *dpdv,
        Float       fu,     /* bump function partial derivatives in uv */
        Float       fv)
{
	Vector tmp1, tmp2;

	VecCross(norm, dpdv, &tmp1);
	VecScale(fu, tmp1, &tmp1);
	VecCross(norm, dpdu, &tmp2);
	VecScale(fv, tmp2, &tmp2);
	VecSub(tmp1, tmp2, &tmp1);
	VecAdd(*norm, tmp1, norm);
	(void)VecNormalize(norm);
}
