/*  NAME:
        RT_Raytrace.c

    DESCRIPTION:
        RayShade surface handling

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaMath.h>
#else
	#include <QD3D.h>
	#include <QD3DMath.h>
#endif

#include "RT.h"
#include "RT_Geometry.h"
#include "RT_DrawContext.h"
#include "RT_Tracer.h"

#include "libobj/geom.h"
#include "libobj/triangle.h"
#include "libshade/rayshade.h"
#include "libshade/picture.h"
#include "libcommon/sampling.h"
#include "libshade/options.h"
#include "libshade/viewing.h"

#if defined(Q3_PROFILE) && Q3_PROFILE
#include <profiler.h>
#endif



/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/

int kRTSamples_Unsampled 			= -1;
int kRTSamples_Supersampled			= -2;

typedef struct TRTScanline {
	Pixel		*pixelValues;
	int			*numberOfSamples;
} TRTScanline;

typedef struct TRTRayTracer {
	TRTDrawContext			*drawContext;
	
	SampleInfo				sampling;
	const int				*sampleNumbers;
	
	Ray						topRay;	/* Top-level ray. */
	
	int						width;
	int						height;
	
	TRTScanline				scan0;
	TRTScanline				scan1;
	TRTScanline				scan2;
} TRTRayTracer;

/******************************************************************************
 **																			 **
 **								Constants								     **
 **																			 **
 *****************************************************************************/

/*
 * "Dither matrices" used to encode the 'number' of a ray that samples a
 * particular portion of a pixel.  Hand-coding is ugly, but...
 */
const int kOneSample[1] 	= 	{0};
const int kTwoSamples[4] 	=	{0, 2, 
								 3, 1};
const int kThreeSamples[9] 	=	{0, 2, 7,
								 6, 5, 1,
				 				 3, 8, 4};
const int kFourSamples[16] 	=	{0,  8,  2, 10,
				 				12,  4, 14,  6,
				  				 3, 11,  1,  9,
				 				15,  7, 13,  5};
const int kFiveSamples[25] 	=	{ 0,  8, 23, 17,  2,
				 				 19, 12,  4, 20, 15,
				  				  3, 21, 16,  9,  6,
				 				 14, 10, 24,  1, 13,
				 				 22,  7, 18, 11,  5};
const int kSixSamples[36]	=	{ 6, 32,  3, 34, 35,  1,
				  				  7, 11, 27, 28,  8, 30,
								 24, 14, 16, 15, 23, 19,
								 13, 20, 22, 21, 17, 18,
								 25, 29, 10,  9, 26, 12,
								 36,  5, 33,  4,  2, 31};
const int kSevenSamples[49] =	{22, 47, 16, 41, 10, 35,  4,
								  5, 23, 48, 17, 42, 11, 29,
								 30,  6, 24, 49, 18, 36, 12,
								 13, 31,  7, 25, 43, 19, 37,
								 38, 14, 32,  1, 26, 44, 20,
								 21, 39,  8, 33,  2, 27, 45,
								 46, 15, 40,  9, 34,  3, 28};
const int kEightSamples[64] =	{ 8, 58, 59,  5,  4, 62, 63,  1,
								 49, 15, 14, 52, 53, 11, 10, 56,
								 41, 23, 22, 44, 45, 19, 18, 48,
								 32, 34, 35, 29, 28, 38, 39, 25,
								 40, 26, 27, 37, 36, 30, 31, 33,
								 17, 47, 46, 20, 21, 43, 42, 24,
								  9, 55, 54, 12, 13, 51, 50, 16,
								 64,  2,  3, 61, 60,  6,  7, 57};



/*===========================================================================*\
 *
 *	Routine:	rt_FullySamplePixel()
 *
 *	Comments:	Samples a pixel, until it's supersampled
 *
\*===========================================================================*/
static TQ3Status
rt_FullySamplePixel(
			TRTRayTracer		*inTracer,
			int					xp,
			int					yp,
			Pixel 				*ioPixel,
			int	 				*ioSample)
{
	Float upos, vpos, u, v;
	int x=0, y, theSampleNum;
	Pixel color;
	
	/*
	 * Done, if already supersampled.
	 */
	if (*ioSample == kRTSamples_Supersampled)
		return kQ3Success; 
		
	if (*ioSample == kRTSamples_Unsampled) {
		/*
		 * No previous sample, initialize to blank.
		 */
		ioPixel->r = 0.0;
		ioPixel->g = 0.0;
		ioPixel->b = 0.0;
		ioPixel->alpha = 0.0;
	}
	else
	{
		if (inTracer->sampling.sidesamples == 1) {
			*ioSample = kRTSamples_Supersampled;
			return kQ3Success;
		}
		x = *ioSample % inTracer->sampling.sidesamples;
		y = *ioSample / inTracer->sampling.sidesamples;
		
		ioPixel->r 		*= inTracer->sampling.filter[x][y];
		ioPixel->g 		*= inTracer->sampling.filter[x][y];
		ioPixel->b 		*= inTracer->sampling.filter[x][y];
		ioPixel->alpha 	*= inTracer->sampling.filter[x][y];
	}
	
	theSampleNum = 0;
	xp += 0;
	vpos = 0+yp-0.5*inTracer->sampling.filterwidth;
	for (y = 0; y < inTracer->sampling.sidesamples; 
					y++, vpos+= inTracer->sampling.filterdelta)
	{
		upos = xp - 0.5*inTracer->sampling.filterwidth;
		for (x = 0; x < inTracer->sampling.sidesamples; x++,
				upos += inTracer->sampling.filterdelta) 
		{
			if (theSampleNum != *ioSample) {
				if (Options.jitter) {
					u = upos + nrand()*inTracer->sampling.filterdelta;
					v = vpos + nrand()*inTracer->sampling.filterdelta;
				} else {
					u = upos;
					v = vpos;
				}
				
				SampleScreen(u, v, &inTracer->topRay, &color,
					inTracer->sampleNumbers[theSampleNum]);
				
				ioPixel->r += color.r*inTracer->sampling.filter[x][y];
				ioPixel->g += color.g*inTracer->sampling.filter[x][y];
				ioPixel->b += color.b*inTracer->sampling.filter[x][y];
				ioPixel->alpha += color.alpha*inTracer->sampling.filter[x][y];
			}
			if (++theSampleNum == inTracer->sampling.totsamples)
				theSampleNum = 0;
		}	
	}	
	if (Options.samplemap)
		ioPixel->alpha = 255;
	
	*ioSample = kRTSamples_Supersampled;
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	rt_FullySamplePixel()
 *
 *	Comments:	Samples a scanline until it's supersampled
 *
\*===========================================================================*/
static void
rt_FullySampleScanline(
		TRTRayTracer		*inTracer,
		int 				y, 
		TRTScanline 		*outScanline)
{
	int x;

	for (x = 0; x < inTracer->width; x++) {
		outScanline->numberOfSamples[x] = kRTSamples_Unsampled;
		rt_FullySamplePixel(inTracer,x, y, 
				&outScanline->pixelValues[x], 
				&outScanline->numberOfSamples[x]);
	}
}
/*===========================================================================*\
 *
 *	Routine:	rt_SingleSamplePixel()
 *
 *	Comments:	Samples a scanline once per pixel
 *
\*===========================================================================*/
static void
rt_SingleSampleScanline(
				TRTRayTracer		*inTracer,
				int 				y, 
				TRTScanline 		*inScanline)
{
	Float upos, vpos, yp;
	int x, usamp, vsamp;

	yp = y - 0.5*inTracer->sampling.filterwidth;
	for (x = 0; x < inTracer->width; x++) {
		/*
		 * Pick a sample number...
		 */
		inScanline->numberOfSamples[x] = nrand() * inTracer->sampling.totsamples;
		/*
		 * Take sample corresponding to sample #.
		 */
		usamp = inScanline->numberOfSamples[x] % inTracer->sampling.sidesamples;
		vsamp = inScanline->numberOfSamples[x] / inTracer->sampling.sidesamples;

		vpos = yp + vsamp * inTracer->sampling.filterdelta;
		upos = x - 0.5*inTracer->sampling.filterwidth +
				usamp*inTracer->sampling.filterdelta;
		if (Options.jitter) {
			vpos += nrand()*inTracer->sampling.filterdelta;
			upos += nrand()*inTracer->sampling.filterdelta;
		}
		
		SampleScreen(upos, vpos, &inTracer->topRay,
			&inScanline->pixelValues[x], inTracer->sampleNumbers[ inScanline->numberOfSamples[x]]);
		if (Options.samplemap)
			inScanline->pixelValues[x].alpha = 0;
	}	
}

/*===========================================================================*\
 *
 *	Routine:	rt_ExcessiveContrast()
 *
 *	Comments:	Returns true if one pixel is so different than it's neigboorhood,
 *				that resampling is needed.
 *
\*===========================================================================*/
static bool
rt_ExcessiveContrast(
		TRTRayTracer	* /*inTracer*/,
        int         	x, 
        Pixel       	pix0[],
        Pixel       	pix1[],
        Pixel       	pix2[])
{
	Float mini, maxi, sum, diff;

	maxi = max(pix0[x].r, pix1[x-1].r);
	if (pix1[x].r > maxi) maxi = pix1[x].r;
	if (pix1[x+1].r > maxi) maxi = pix1[x+1].r;
	if (pix2[x].r > maxi) maxi = pix2[x].r;

	mini = min(pix0[x].r, pix1[x-1].r);
	if (pix1[x].r < mini) mini = pix1[x].r;
	if (pix1[x+1].r < mini) mini = pix1[x+1].r;
	if (pix2[x].r < mini) mini = pix2[x].r;

	diff = maxi - mini;
	sum = maxi + mini;
	if (sum > EPSILON && diff/sum > Options.contrast.r)
		return true;

	maxi = max(pix0[x].g, pix1[x-1].g);
	if (pix1[x].g > maxi) maxi = pix1[x].g;
	if (pix1[x+1].g > maxi) maxi = pix1[x+1].g;
	if (pix2[x].g > maxi) maxi = pix2[x].g;

	mini = min(pix0[x].g, pix1[x-1].g);
	if (pix1[x].g < mini) mini = pix1[x].g;
	if (pix1[x+1].g < mini) mini = pix1[x+1].g;
	if (pix2[x].g < mini) mini = pix2[x].g;

	diff = maxi - mini;
	sum = maxi + mini;

	if (sum > EPSILON && diff/sum > Options.contrast.g)
		return true;

	maxi = max(pix0[x].b, pix1[x-1].b);
	if (pix1[x].b > maxi) maxi = pix1[x].b;
	if (pix1[x+1].b > maxi) maxi = pix1[x+1].b;
	if (pix2[x].b > maxi) maxi = pix2[x].b;

	mini = min(pix0[x].b, pix1[x-1].b);
	if (pix1[x].b < mini) mini = pix1[x].b;
	if (pix1[x+1].b < mini) mini = pix1[x+1].b;
	if (pix2[x].b < mini) mini = pix2[x].b;

	diff = maxi - mini;
	sum = maxi + mini;
	if (sum > EPSILON && diff/sum > Options.contrast.b)
		return true;

	return false;
}
/*===========================================================================*\
 *
 *	Routine:	rt_AdaptiveRefineScanline()
 *
 *	Comments:	Adaptive refines a scanline until there is no Excessive 
 *				Contrast.
 *
\*===========================================================================*/
static void
rt_AdaptiveRefineScanline(
		TRTRayTracer	*inTracer,
        int            	y, 
        TRTScanline    	*scan0, 
        TRTScanline    	*scan1,
        TRTScanline    	*scan2)
{
	int x;
	bool done;

	/*
	 * Walk down scan1, looking at 4-neighbors for excessive contrast.
	 * If found, supersample *all* neighbors not already supersampled.
	 * The process is repeated until either there are no
	 * high-contrast regions or all such regions are already supersampled.
	 */

	do {
		done = true;
		for (x = 1; x < inTracer->width -1; x++) {
			/*
		 	 * Find min and max RGB for area we care about
			 */
			if (rt_ExcessiveContrast(inTracer,x, scan0->pixelValues, scan1->pixelValues,
			    scan2->pixelValues)) {
				if (scan1->numberOfSamples[x-1] != kRTSamples_Supersampled) {
					done = false;
					rt_FullySamplePixel(
						inTracer,
						x-1, y,
						&scan1->pixelValues[x-1],
						&scan1->numberOfSamples[x-1]);
				}
				if (scan0->numberOfSamples[x] != kRTSamples_Supersampled) {
					done = false;
					rt_FullySamplePixel(
						inTracer,
						x, y-1,
						&scan0->pixelValues[x],
						&scan0->numberOfSamples[x]);
				}
				if (scan1->numberOfSamples[x+1] != kRTSamples_Supersampled) {
					done = false;
					rt_FullySamplePixel(
						inTracer,
						x+1, y,
						&scan1->pixelValues[x+1],
						&scan1->numberOfSamples[x+1]);
				}
				if (scan2->numberOfSamples[x] != kRTSamples_Supersampled) {
					done = false;
					rt_FullySamplePixel(
						inTracer,
						x, y+1,
						&scan2->pixelValues[x],
						&scan2->numberOfSamples[x]);
				}
				if (scan1->numberOfSamples[x] != kRTSamples_Supersampled) {
					done = false;
					rt_FullySamplePixel(
						inTracer,
						x, y,
						&scan1->pixelValues[x],
						&scan1->numberOfSamples[x]);
				}
			}
		}
	} while (!done);
}

/*===========================================================================*\
 *
 *	Routine:	RTRayTracer_Create()
 *
 *	Comments:	Creates a ray tracer object
 *
\*===========================================================================*/
TRTRayTracer*
RTRayTracer_Create(
					TRTDrawContext	* /*inDrawContext*/,
					int				width,
					int				height)
{
	TRTRayTracer	*result = NULL;
	
	result = (TRTRayTracer*)calloc(1,sizeof(TRTRayTracer));
	if (result == NULL)
		return NULL;
#if defined(Q3_PROFILE) && Q3_PROFILE
	ProfilerInit(collectDetailed,bestTimeBase,1000,100);
#endif	

	/*
	 *	
	 */
	Options.resolution_set = TRUE;
	Screen.xres = width;
	Screen.yres = height;
	result->width = width;
	result->height = height;
   
    /*
     * Set sampling options.
     */
    SamplingSetOptions(Options.samples, Options.gaussian,
    	Options.filterwidth);
    /*
     * Camera is currently static; initialize it here.
     */
    RSViewing();
    
    /*
     * If world is not set up abort.
     */
	
	/*
	 * Create the data needed for RayTrace:
	 */
	result->sampling = Sampling;
	switch(Sampling.sidesamples) {
		case 1:
			result->sampleNumbers = kOneSample;
			break;
		case 2:
			result->sampleNumbers = kTwoSamples;
			break;
		case 3:
			result->sampleNumbers = kThreeSamples;
			break;
		case 4:
			result->sampleNumbers = kFourSamples;
			break;
		case 5:
			result->sampleNumbers = kFiveSamples;
			break;
		case 6:
			result->sampleNumbers = kSixSamples;
			break;
		case 7:
			result->sampleNumbers = kSevenSamples;
			break;
		case 8:
			result->sampleNumbers = kEightSamples;
			break;
		default:
			goto cleanup;
			break;
	}
	/*
 	 * Allocate pixel arrays and arrays to store sampling info.
 	 */
	result->scan0.pixelValues = (Pixel *)malloc(width * sizeof(Pixel));
	if (!result->scan0.pixelValues)	goto cleanup;
	result->scan1.pixelValues = (Pixel *)malloc(width * sizeof(Pixel));
	if (!result->scan1.pixelValues)	goto cleanup;
	result->scan2.pixelValues = (Pixel *)malloc(width * sizeof(Pixel));
	if (!result->scan2.pixelValues)	goto cleanup;

	result->scan0.numberOfSamples = (int *)malloc(width * sizeof(int));
	if (!result->scan0.numberOfSamples)	goto cleanup;
	result->scan1.numberOfSamples = (int *)malloc(width * sizeof(int));
	if (!result->scan1.numberOfSamples)	goto cleanup;
	result->scan2.numberOfSamples = (int *)malloc(width * sizeof(int));
	if (!result->scan2.numberOfSamples)	goto cleanup;
    
    /*
	 * The top-level ray TopRay always has as its origin the
	 * eye position and as its medium NULL, indicating that it
	 * is passing through a medium with index of refraction
	 * equal to DefIndex.
	 */
	result->topRay.pos = Camera.pos;
	result->topRay.media = (Medium *)0;
	result->topRay.depth = 0;

	/*
	 * Always fully sample the bottom and top rows and the left
	 * and right column of pixels.  This minimizes artifacts that
	 * may arise when piecing together images.
	 */
	rt_FullySampleScanline(result,0, &result->scan0);		
	rt_SingleSampleScanline(result,1, &result->scan1);
	rt_FullySamplePixel(result,0, 1, &result->scan1.pixelValues[0], &result->scan1.numberOfSamples[0]);
	rt_FullySamplePixel(result,result->width -1, 1, &result->scan1.pixelValues[result->width -1],
		&result->scan1.numberOfSamples[result->width -1]);
    	

	
	return result;
cleanup:
	if (result) RTRayTracer_Delete(result);
	return NULL;
}
/*===========================================================================*\
 *
 *	Routine:	RTRayTracer_ScanNextLine()
 *
 *	Comments:	Scans the next line.
 *
\*===========================================================================*/
TQ3Status
RTRayTracer_ScanNextLine(
					TRTRayTracer 	*inTracer,
					int 			inCurrentLine,
					TQ3Float32		outBuffer[][4],
					int				inBufferSize)
{
	int 	i;
	Pixel	*tmpPixel;
	int		*tmpSample;
	int		y;
	
	if ( inBufferSize < ( inTracer->width * 4 * sizeof ( TQ3Float32 ) ) )
		return kQ3Failure;
	
	y = inCurrentLine+1;
	if (1 <= y && y <= inTracer->height)
	{
		rt_SingleSampleScanline(inTracer,y+1, &inTracer->scan2);
		rt_FullySamplePixel(inTracer,0, y+1, &inTracer->scan2.pixelValues[0], 
											 &inTracer->scan2.numberOfSamples[0]);
		rt_FullySamplePixel(inTracer,inTracer->width -1, y+1,
							&inTracer->scan2.pixelValues[inTracer->width -1],
							&inTracer->scan2.numberOfSamples[inTracer->width -1]);
		if (inTracer->sampling.sidesamples > 1)
			rt_AdaptiveRefineScanline(inTracer,y,&inTracer->scan0,&inTracer->scan1,&inTracer->scan2);
	
		for (i = 0; i < inTracer->width; i++)
    	{
	        outBuffer[i][0] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].r ) ;
	        outBuffer[i][1] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].g ) ;
	        outBuffer[i][2] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].b ) ;
	        outBuffer[i][3] = inTracer->scan0.pixelValues[i].alpha ;
	    }	
	    tmpPixel 							= inTracer->scan0.pixelValues;
	    tmpSample 							= inTracer->scan0.numberOfSamples;
	    inTracer->scan0.pixelValues 		= inTracer->scan1.pixelValues;
	    inTracer->scan0.numberOfSamples 	= inTracer->scan1.numberOfSamples;
	    inTracer->scan1.pixelValues 		= inTracer->scan2.pixelValues;
	    inTracer->scan1.numberOfSamples 	= inTracer->scan2.numberOfSamples;
	    inTracer->scan2.pixelValues 		= tmpPixel;
	    inTracer->scan2.numberOfSamples 	= tmpSample;
	}
	else if (y == inTracer->height+1)
	{
		/*
	     * Supersample last scanline.
	     */
	    for (y = 1; y < inTracer->width -1; y++) {
		    if (inTracer->scan0.numberOfSamples[y] != kRTSamples_Supersampled)
			   rt_FullySamplePixel(inTracer,y, inTracer->width -1,
				    &inTracer->scan0.pixelValues[y],
				    &inTracer->scan0.numberOfSamples[y]);
	    }
	    for (i = 0; i < inTracer->width; i++)
    	{
	        outBuffer[i][0] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].r ) ;
	        outBuffer[i][1] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].g ) ;
	        outBuffer[i][2] = GAMMACORRECT ( inTracer->scan0.pixelValues[i].b ) ;
	        outBuffer[i][3] = inTracer->scan0.pixelValues[i].alpha ;
	    }
	}
	else
	{
		return kQ3Failure;
	}
	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RTRayTracer_Delete()
 *
 *	Comments:	Deletes the ray tracer object
 *
\*===========================================================================*/
void
RTRayTracer_Delete(TRTRayTracer *inTracer)
{
	if (!inTracer)
		return;
#if defined(Q3_PROFILE) && Q3_PROFILE
	ProfilerDump("\pRayShade profile");
	ProfilerTerm();
#endif	

	if (inTracer->scan0.pixelValues) free(inTracer->scan0.pixelValues);
	if (inTracer->scan1.pixelValues) free(inTracer->scan1.pixelValues);
	if (inTracer->scan2.pixelValues) free(inTracer->scan2.pixelValues);
	inTracer->scan0.pixelValues = NULL;
	inTracer->scan1.pixelValues = NULL;
	inTracer->scan2.pixelValues = NULL;
	if (inTracer->scan0.numberOfSamples) free(inTracer->scan0.numberOfSamples);
	if (inTracer->scan1.numberOfSamples) free(inTracer->scan1.numberOfSamples);
	if (inTracer->scan2.numberOfSamples) free(inTracer->scan2.numberOfSamples);
	inTracer->scan0.numberOfSamples = NULL; 
	inTracer->scan1.numberOfSamples = NULL;
	inTracer->scan2.numberOfSamples = NULL;
	
	free(inTracer);

}