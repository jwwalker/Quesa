/*  NAME:
        AppDelegate.m

    DESCRIPTION:
        

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
    ___________________________________________________________________________
*/
#import "AppDelegate.h"

#import <Foundation/Foundation.h>

#include <QuesaCocoa/Quesa.h>
#include <QuesaCocoa/QuesaDrawContext.h>
#include <QuesaCocoa/QuesaCamera.h>
#include <QuesaCocoa/QuesaShader.h>
#include <QuesaCocoa/QuesaMath.h>
#include <QuesaCocoa/QuesaView.h>
#include <QuesaCocoa/QuesaMath.h>
#include <QuesaCocoa/QuesaLight.h>
#include <QuesaCocoa/QuesaStyle.h> 
#include <QuesaCocoa/QuesaGroup.h> 
#include <QuesaCocoa/QuesaGeometry.h> 
#include <QuesaCocoa/QuesaTransform.h> 
#include <QuesaCocoa/QuesaRenderer.h> 

#import "Quesa3DView.h"
#import "DemoGeometry.h"



#define kMenuItemGeometryBox								1
#define kMenuItemGeometryCone								2
#define kMenuItemGeometryCylinder							3
#define kMenuItemGeometryDisk								4
#define kMenuItemGeometryEllipse							5
#define kMenuItemGeometryEllipsoid							6
#define kMenuItemGeometryGeneralPolygon						7
#define kMenuItemGeometryLine								8
#define kMenuItemGeometryMarker								9
#define kMenuItemGeometryMesh								10
#define kMenuItemGeometryNURBCurve							11
#define kMenuItemGeometryNURBPatch							12
#define kMenuItemGeometryPixmapMarker						13
#define kMenuItemGeometryPoint								14
#define kMenuItemGeometryPolyLine							15
#define kMenuItemGeometryPolygon							16
#define kMenuItemGeometryPolyhedron							17
#define kMenuItemQuesaLogo									18
#define kMenuItemGeometryTorus								19
#define kMenuItemGeometryTriangle							20
#define kMenuItemGeometryTriGrid							21
#define kMenuItemGeometryTriMesh							22


#define kRendererMaxNum									50

static TQ3SubdivisionStyleData subdivStyle = {kQ3SubdivisionMethodConstant,   30.0f, 30.0f};
static TQ3BackfacingStyle				gSceneBackfacing    = kQ3BackfacingStyleBoth;
static TQ3Object						gSceneGeometry      = NULL;
static TQ3ShaderObject					gSceneIllumination  = NULL;
static TQ3Matrix4x4						gMatrixCurrent;
static TQ3Matrix4x4						gRotationMatrix;
static TQ3Object						gSceneBounds        = NULL;
static BOOL								animation = YES;
static NSTimer*							animationTimer = nil;
static  TQ3ObjectType			gRenderers[kRendererMaxNum];

@implementation AppDelegate

//==================================================================================
//	buildRendererMenu
//==================================================================================

- (void)buildRendererMenu
{	TQ3SubClassData		rendererData;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n, m;
    char				theStr[256];
    
  //build the renderer menu
  [rendererMenu removeAllItems];


	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;

	// If we can find any renderers, add them to the menu
	if (rendererData.numClasses != 0)
    {
      m = 0;    
    
    // Fill the remaining slots
    for (n = 0; n < rendererData.numClasses; n++)
    {
        // Skip the generic renderer, since it can't actually render
        if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
        {
            // Grab the nick name, falling back to the class name if that fails
            qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n], (char *) theStr);
            if (qd3dStatus == kQ3Failure || theStr[0] == 0x00)
                qd3dStatus = Q3ObjectHierarchy_GetStringFromType(rendererData.classTypes[n], (char *) theStr);


            // Add the menu item and save the type
            if (qd3dStatus == kQ3Success)
            {
              if (theStr[0] != 0x00)
              {
                [rendererMenu addItemWithTitle:[NSString stringWithCString:theStr]];
                [[rendererMenu lastItem]setTag:m];
                gRenderers[m++] = rendererData.classTypes[n];
              }
            }
            }
        }
    }
	



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);

    [rendererMenu selectItemAtIndex:[rendererMenu indexOfItemWithTag:1]];
 
}

//==================================================================================
//	applicationDidFinishLaunching
//==================================================================================

- (void)applicationDidFinishLaunching:(NSNotification*)note
{
	[self buildRendererMenu];
    [objectMenu selectItemWithTitle:@"Quesa Logo"];
}

//==================================================================================
//	qd3dViewDidInit
//==================================================================================

-(void)qd3dViewDidInit:(Quesa3DView*)inView
{
    animationTimer = [[NSTimer scheduledTimerWithTimeInterval:0.05f
                                                      target:self
                                                      selector:@selector(animationTimerFired:)
                                                      userInfo:nil
                                                      repeats:YES]retain];

	Q3Matrix4x4_SetIdentity(&gMatrixCurrent);
  	Q3Matrix4x4_SetRotate_XYZ(&gRotationMatrix, 0.03f, 0.05f, 0.005f);
    gSceneGeometry = createGeomQuesa();
    gSceneIllumination = Q3PhongIllumination_New();
   
}

//==================================================================================
//	qd3dViewWillRender
//==================================================================================

-(void)qd3dViewWillRender:(Quesa3DView*)inView
{

}

//==================================================================================
//	qd3dViewRenderFrame
//==================================================================================

-(void)qd3dViewRenderFrame:(Quesa3DView*)inView
{


	// Submit the styles
	Q3BackfacingStyle_Submit(gSceneBackfacing,               [inView qd3dView]);
	Q3InterpolationStyle_Submit(kQ3InterpolationStyleVertex, [inView qd3dView]);
	Q3SubdivisionStyle_Submit(&subdivStyle,                  [inView qd3dView]);



	// Submit the scene
	Q3Shader_Submit(gSceneIllumination, [inView qd3dView]);
//	if (gShowTexture && gSceneTexture != NULL)
//		Q3Shader_Submit(gSceneTexture, [inView qd3dView]);
		
	Q3MatrixTransform_Submit(&gMatrixCurrent, [inView qd3dView]);
	Q3Object_Submit(gSceneGeometry, [inView qd3dView]);

	if (gSceneBounds != NULL)
    {
      Q3BackfacingStyle_Submit(kQ3BackfacingStyleBoth, [inView qd3dView]);
      Q3Object_Submit(gSceneBounds, [inView qd3dView]);
    }

}

//==================================================================================
//	qd3dViewDidRender
//==================================================================================

-(void)qd3dViewDidRender:(Quesa3DView*)inView
{

}

//==================================================================================
//	animationTimerFired
//==================================================================================

- (void)animationTimerFired:(NSTimer*)timer
{
  if(!animation)
    return;
    
  Q3Matrix4x4_SetRotate_XYZ(&gRotationMatrix, 0.03f, 0.05f, 0.005f);
  Q3Matrix4x4_Multiply(&gMatrixCurrent, &gRotationMatrix, &gMatrixCurrent);
  [quesa3dView setNeedsDisplay:YES];

}

//IB outlet actions to implement the controls

//==================================================================================
//	setXRot
//==================================================================================

- (void)setXRot:(id)sender
{
  float sliderVal = [sender floatValue];
  Q3Matrix4x4_SetRotate_XYZ(&gMatrixCurrent, Q3Math_DegreesToRadians(sliderVal), 0.0f, 0.0f);
  [quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	setYRot
//==================================================================================

- (void)setYRot:(id)sender
{
  float sliderVal = [sender floatValue];
	Q3Matrix4x4_SetRotate_XYZ(&gMatrixCurrent, 0.0f, Q3Math_DegreesToRadians(sliderVal), 0.0f);
  [quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	setZRot
//==================================================================================

- (void)setZRot:(id)sender
{
  float sliderVal = [sender floatValue];
  Q3Matrix4x4_SetRotate_XYZ(&gMatrixCurrent, 0.0f, 0.0f, Q3Math_DegreesToRadians(sliderVal));
  [quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	setGeometryFromTag
//==================================================================================

-(void)setGeometryFromTag:(id)sender
{
  int tagVal = [[sender selectedItem]tag];
  TQ3GeometryObject theGeom = NULL;
  
  switch(tagVal)
  {
      case kMenuItemGeometryBox:
          theGeom = createGeomBox();
          break;

      case kMenuItemGeometryCone:
          theGeom = createGeomCone();
          break;

      case kMenuItemGeometryCylinder:
          theGeom = createGeomCylinder();
          break;

      case kMenuItemGeometryDisk:
          theGeom = createGeomDisk();
          break;

      case kMenuItemGeometryEllipse:
          theGeom = createGeomEllipse();
          break;

      case kMenuItemGeometryEllipsoid:
          theGeom = createGeomEllipsoid();
          break;

      case kMenuItemGeometryGeneralPolygon:
          theGeom = createGeomGeneralPolygon();
          break;

      case kMenuItemGeometryLine:
          theGeom = createGeomLine();
          break;

      case kMenuItemGeometryMarker:
          theGeom = createGeomMarker();
          break;

      case kMenuItemGeometryMesh:
          theGeom = createGeomMesh();
          break;

      case kMenuItemGeometryNURBCurve:
          theGeom = createGeomNURBCurve();
          break;

      case kMenuItemGeometryNURBPatch:
          theGeom = createGeomNURBPatch();
          break;

      case kMenuItemGeometryPixmapMarker:
          theGeom = createGeomPixmapMarker();
          break;

      case kMenuItemGeometryPoint:
          theGeom = createGeomPoint();
          break;

      case kMenuItemGeometryPolyLine:
          theGeom = createGeomPolyLine();
          break;

      case kMenuItemGeometryPolygon:
          theGeom = createGeomPolygon();
          break;
      
      case kMenuItemGeometryPolyhedron:
          theGeom = createGeomPolyhedron();
          break;
      
      case kMenuItemQuesaLogo:
          theGeom = createGeomQuesa();
          break;

      case kMenuItemGeometryTriangle:
          theGeom = createGeomTriangle();
          break;

      case kMenuItemGeometryTorus:
          theGeom = createGeomTorus();
          break;

      case kMenuItemGeometryTriGrid:
          theGeom = createGeomTriGrid();
          break;

      case kMenuItemGeometryTriMesh:
          theGeom = createGeomTriMesh();
          break;

      default:
          break;
  }



  // If we created it, replace the current geometry
  if (theGeom != NULL)
  {
      if (gSceneGeometry != NULL)
          Q3Object_Dispose(gSceneGeometry);
      
      gSceneGeometry = theGeom;

      if (gSceneBounds != NULL)
      {
          Q3Object_Dispose(gSceneBounds);
          gSceneBounds = createGeomBounds(gSceneGeometry);
      }
  }
          
  Q3Matrix4x4_SetIdentity(&gMatrixCurrent);
  [quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	toggleAnimation
//==================================================================================

- (void)toggleAnimation:(id)sender
{
    animation = !animation;
}

//==================================================================================
//	setRendererFromTag
//==================================================================================

- (void)setRendererFromTag:(id)sender
{
  int tag = [[sender selectedItem]tag];
  Q3View_SetRendererByType([quesa3dView qd3dView], gRenderers[tag]);
  [quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	toggleBoundingBox
//==================================================================================

- (void)toggleBoundingBox:(id)sender
{
  // Create or dispose of the bounding geometry
  if (gSceneBounds == NULL)
      gSceneBounds = createGeomBounds(gSceneGeometry);
  else
  {
      Q3Object_Dispose(gSceneBounds);
      gSceneBounds = NULL;
  }
  [quesa3dView setNeedsDisplay:YES];
}
@end
