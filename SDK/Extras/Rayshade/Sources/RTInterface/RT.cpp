/*  NAME:
        RT.c

    DESCRIPTION:
        A procedural api for RayShade.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RT.h"
#include "RT_Light.h"
#include "RT_DrawContext.h"

#include "libshade/rayshade.h"
#include "libshade/viewing.h"
#include "libshade/defaults.h"
#include "libshade/options.h"
#include "libshade/stats.h"
#include "libsurf/atmosphere.h"
#include "libobj/list.h"
#include "liblight/light.h"
#include "libobj/grid.h"

#include <stdlib.h>
#include <stdio.h>

#if USE_QUESA_INCLUDES
	#include <QuesaMath.h>
#else
	#include <QD3DMath.h>
#endif

#if Q3_PROFILE
#include <profiler.h>
#endif

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

/******************************************************************************
 **																			 **
 **								Global variables						     **
 **																			 **
 *****************************************************************************/
RSOptions	Options;

/*===========================================================================*\
 *
 *	Routine:	RT_Create()
 *
 *	Comments:	Creates a new raytracer object.
 *
\*===========================================================================*/
TRTDrawContext		*RT_Create(void)
{
	try {
		
		
		TRTDrawContext*		theDrawContext;
	
		theDrawContext = new TRTDrawContext;
	
		if (theDrawContext == NULL)
			return NULL;
	
		theDrawContext->definedSurfaces = NULL;
		theDrawContext->currentSurface = NULL;
		
		theDrawContext->objects = NULL;
		theDrawContext->currentTexture = NULL;
	
		return theDrawContext;
	}
	catch(...)
	{
		return NULL;
	}
}
/*===========================================================================*\
 *
 *	Routine:	RT_Delete()
 *
 *	Comments:	Deletes the RayTracer, 
 *  
 *  ToDo: 		Implement this fully
 *
\*===========================================================================*/
void	
RT_Delete(TRTDrawContext *inContext)
{
	RT_Reset(inContext);
	
	if (!inContext)
		return;
		
	delete inContext;
}
/*===========================================================================*\
 *
 *	Routine:	rt_Cleanup()
 *
 *	Comments:	Cleans up the parameters
 *
\*===========================================================================*/
static
TQ3Status
rt_Cleanup(TRTDrawContext	* /*inContext*/)
{
	extern Light *Lights;
	extern void OpenStatsFile();

	Options.quiet = 1;
	
	if (Options.samples == UNSET)
		Options.samples = DEFSAMPLES;

	if (Options.filterwidth == UNSET) {
		if (Options.gaussian)
			Options.filterwidth = FILTERWIDTH;
		else
			/* Default box filter width of 1.0 */
			Options.filterwidth = 1.0;
	}

	Options.endframe = Options.startframe + Options.totalframes -1;

	OpenStatsFile();

	ViewingSetup();

	if (Options.cutoff.r == UNSET)
		Options.cutoff.r = Options.cutoff.g =
			Options.cutoff.b = DEFCUTOFF;

	/*
	 * Set contrast.
	 */
	if (Options.contrast.r == UNSET) {
		Options.contrast.r = DEFREDCONT;
		Options.contrast.g = DEFGREENCONT;
		Options.contrast.b = DEFBLUECONT;
	}

	/*
	 * Image gamma is inverse of display gamma.
	 */
	if (fabs(Options.gamma) > EPSILON)
		Options.gamma = 1. / Options.gamma;
	else
		Options.gamma = FAR_AWAY;

	if (Options.maxdepth < 0)
		Options.maxdepth = 0;


	LightSetup();
	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	rt_Setup()
 *
 *	Comments:	Sets the default parameters
 *
\*===========================================================================*/
static
TQ3Status
rt_Setup(TRTDrawContext * /*inContext*/)
{
	extern Medium TopMedium;
	extern void NoiseInit();
#ifdef MULTIMAX
	unsigned int bytes;

	/*
	 * Initialize shared memory stuff.
	 */
	bytes = 1 << SHARED_BYTES;
	if (share_malloc_init(bytes) == -1) {
		RLerror(RL_PANIC, "Cannot share_malloc %d bytes.\n",bytes);
	} else
		fprintf(fstats,"Malloced %d bytes of shared memory.\n",
				bytes);
#endif

	Camera.hfov = HFOV;
	Camera.vfov = UNSET;
	Camera.pos.x = EYEX;
	Camera.pos.y = EYEY;
	Camera.pos.z = EYEZ;
	Camera.lookp.x = LOOKX;
	Camera.lookp.y = LOOKY;
	Camera.lookp.z = LOOKZ;
	Camera.up.x = UPX;
	Camera.up.y = UPY;
	Camera.up.z = UPZ;
	Camera.focaldist = UNSET;
	Camera.aperture = 0.;

	Screen.xres = Screen.yres = UNSET;

	Options.cpp = TRUE;
	Options.maxdepth = MAXDEPTH;
	Options.report_freq = REPORTFREQ;
	Options.jitter = TRUE;
	Options.samples = UNSET;
	Options.gaussian = GAUSSIAN;
	Options.filterwidth = UNSET;
	Options.contrast.r = UNSET;
	Options.ambient.r = Options.ambient.g =
		Options.ambient.b = 1.0;
	Options.cutoff.r = UNSET;
	Options.cache = TRUE;
	Options.shadowtransp = TRUE;
	Options.crop[LOW][X] = Options.crop[LOW][Y] = 0.;
	Options.crop[HIGH][X] = Options.crop[HIGH][Y] = 1.;
	Stats.fstats = stderr;
	Options.pictfile = stdout;
#ifdef URT
	Options.alpha = TRUE;
	Options.exp_output = FALSE;
#endif
	Options.gamma = GAMMA;
	Options.eyesep = UNSET;
#ifdef LINDA
	Options.workers = WORKERS;
#endif

	Options.totalframes = 1;
	Options.startframe = 0;
	Options.starttime = 0.;
	Options.framelength = 1.;
	Options.shutterspeed = 0.;

	TopMedium.index = DEFAULT_INDEX;
	TopMedium.statten = 1.0;
	NoiseInit();			/* Initialize values for Noise() */


	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RT_Begin()
 *
 *	Comments:	Prepares for rendering to the screen.
 *  
 *  ToDo: 		Implement this fully
 *
\*===========================================================================*/
TQ3Status	
RT_BeginScene(TRTDrawContext *inContext)
{
	TQ3Status theStatus;
	
	if (!inContext)
		return kQ3Failure;
	/*
	 * Reset any objects/ lights:
	 */
	RT_Reset(inContext);
   	/*
 	 * Initialize variables, etc.
	 */
 	theStatus = rt_Setup(inContext);
 	if (theStatus != kQ3Success)
 		return theStatus;
 	
	/*
     * Set the ambient light to black 
     */
   	Options.ambient.r = 0.0;
   	Options.ambient.g = 0.0;
   	Options.ambient.b = 0.0;
   	

	return kQ3Success;	
}
/*===========================================================================*\
 *
 *	Routine:	RT_EndScene()
 *
 *	Comments:	Closes the scene, no more object can be submitted after this.
 *  
 *  ToDo: 		Implement this fully
 *
\*===========================================================================*/

extern Geom* World;

TQ3Status	
RT_EndScene(TRTDrawContext *inContext)
{
	TQ3Status 	theStatus;
	Geom		*theGrid;
	Geom		*theList;
	if (!inContext)
		return kQ3Failure;


	/*
	 * For efficiency enclose the topmost item into a grid:
	 */
	theGrid = GeomGridCreate(25,25,25);
	if (!theGrid)
		return kQ3Failure;
	AggregateConvert(theGrid,inContext->objects);
	
	inContext->objects = theGrid;
	
	/*
	 * The topmost item should be a list. 
	 */
	theList = GeomListCreate();
	if (!theList)
		return kQ3Failure;
	AggregateConvert(theList,inContext->objects);
	inContext->objects = theList;
	
	/*
	 * Set the top level object to the world.
	 */
	GeomComputeBounds(theList);
	World = theList;
	
	theStatus = rt_Cleanup(inContext);
	if (theStatus == kQ3Failure)
		return kQ3Failure;
	

	
	return kQ3Success;	
}
/*===========================================================================*\
 *
 *	Routine:	RT_Reset()
 *
 *	Comments:	Destroys objects, etc. in the scene.
 *	
 *  ToDo: Implement this fully.
 *
\*===========================================================================*/
TQ3Status	
RT_Reset(TRTDrawContext *inContext)
{
	TQ3Status result;
	if (!inContext)
		return kQ3Failure;
		
	GeomDeleteEvery(inContext->objects);
	inContext->objects = NULL;
	Options.ambient.r = 0;
    Options.ambient.g = 0;
    Options.ambient.b = 0;
    
    SurfaceDeleteEvery(inContext->definedSurfaces);
    inContext->definedSurfaces = NULL;
		
	result = RT_ResetLights(inContext);
	
	/*
	 * Delete all defined texture:
	 */
	std::map<int,CTexture*>::iterator p = inContext->definedTextures.begin();
	while (p != inContext->definedTextures.end())
	{
		delete (*p).second;
		p++;
	}
	inContext->definedTextures.erase(inContext->definedTextures.begin(),
									 inContext->definedTextures.end());
	
	return result;	
}


/*===========================================================================*\
 *
 *	Routine:	RT_SetCamera()
 *
 *	Comments:	Sets the camera
 *
\*===========================================================================*/
TQ3Status RT_SetCamera(TRTDrawContext * /*inContext*/, 
			TQ3Point3D *inCameraLocation,TQ3Point3D *inPointOfInterests,
			TQ3Vector3D *inUpVector,double minfov,
			double aspectRatioXToY)
{
    Camera.pos.x = inCameraLocation->x;
    Camera.pos.y = inCameraLocation->y;
    Camera.pos.z = inCameraLocation->z;
    
    Camera.up.x = inUpVector->x;
    Camera.up.y = inUpVector->y;
    Camera.up.z = inUpVector->z;
    
    Camera.lookp.x = inPointOfInterests->x;
    Camera.lookp.y = inPointOfInterests->y;
    Camera.lookp.z = inPointOfInterests->z;
    
    /*
     * Calculate hfov and vfov
     */
    if (aspectRatioXToY <= 1.0)
    {
    	Camera.hfov = Q3Math_RadiansToDegrees(minfov);
   		Camera.vfov = Q3Math_RadiansToDegrees(atan2f(tan(minfov),aspectRatioXToY));
   	}
   	else
   	{
   		Camera.vfov = Q3Math_RadiansToDegrees(minfov);
   		Camera.hfov = Q3Math_RadiansToDegrees(atanf(tan(minfov)*aspectRatioXToY));
   	} 
    return kQ3Success;
}