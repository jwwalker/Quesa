
/*  NAME:
        QGScene.cpp
        
    DESCRIPTION:
		The 'bag' which contains all the objects used in a game.
		uses SimpleVector (see SimpleVector.h) to store references to objects
		also holds references to camera and lights for ease of access

    COPYRIGHT:
        Queeg/Quesa Copyright © 1999-2001, Queeg/Quesa Developers.
        
        For the list of Queeg/Quesa Developers, and contact details, see:
        
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


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QGScene.h"
#include "QGObject.h"
#include "QGTriMesh.h"
#include "QGCamera.h"

#include "QuesaDrawContext.h"
#include "QuesaRenderer.h"
#include "QuesaShader.h"
#include "QuesaCamera.h"
#include "QuesaLight.h"
#include "QuesaGeometry.h"
#include "QuesaGroup.h"
#include "QuesaMath.h"
#include "QuesaSet.h"
#include "QuesaTransform.h"





//=============================================================================
//      QGScene::QGScene : Constructor
//-----------------------------------------------------------------------------
QGScene::QGScene()
: mView(NULL), mInterpolation(0), mBackFacing(0),
  mFillStyle(0), mShader(0), mBackground(0), mCamera(NULL)
{
}

//=============================================================================
//      QGScene::QGScene : Destructor - unmakes all objects and styles
//-----------------------------------------------------------------------------
QGScene::~QGScene()
{
	VecReverseIterate(i, mObjects) {
		delete mObjects[i];
	}

	if (mBackground) {
		mBackground->mScene = NULL;
		delete mBackground;
	}

	if (mInterpolation) {
		Q3Object_Dispose(mInterpolation);
		Q3Object_Dispose(mBackFacing);
		Q3Object_Dispose(mFillStyle);
		if (mShader) Q3Object_Dispose(mShader);
	}
	
}

//=============================================================================
//      QGScene::SetPrefs : Set the various options for the scene
//-----------------------------------------------------------------------------
void QGScene::SetPrefs(	TQ3InterpolationStyle interpolation,
					TQ3BackfacingStyle backfacing,
					TQ3FillStyle fillStyle,
					IlluminationStyle illumination )
{
	if (mInterpolation) {
		Q3Object_Dispose(mInterpolation);
		Q3Object_Dispose(mBackFacing);
		Q3Object_Dispose(mFillStyle);
		Q3Object_Dispose(mShader);
	}

	mInterpolation = Q3InterpolationStyle_New(interpolation) ;
	mBackFacing = Q3BackfacingStyle_New(backfacing ) ;
	mFillStyle = Q3FillStyle_New(fillStyle ) ;
	
	// (note: might want to do this one on an object-by-object basis)
	switch (illumination) {
		case kPhongIllumination:	mShader = Q3PhongIllumination_New();
									break;
		case kLambertIllumination:	mShader = Q3LambertIllumination_New();
									break;
		default:
		case kNoIllumination:		mShader = 0;	// mShader = Q3NULLIllumination_New();
									break;
	}
}

//=============================================================================
//      QGScene::SetView : Set the view used by the scene to render
//-----------------------------------------------------------------------------
void QGScene::SetView( TQ3ViewObject view )
{
	mView = view;

	if (!mInterpolation) SetPrefs( kQ3InterpolationStyleVertex, kQ3BackfacingStyleBoth,
					kQ3FillStyleFilled, kPhongIllumination );

	// create a camera object
	TQ3CameraObject camera;
	Q3View_GetCamera( mView, &camera );
	mCamera = new QGCamera(camera);
}

//=============================================================================
//      QGScene::SetBackground : Set the background object - used for invariant backgrounds
//-----------------------------------------------------------------------------
void QGScene::SetBackground( QGObject *bkgnd )
{
	// if there was a previous background, delete it now
	if (mBackground) {
		mBackground->mScene = NULL;
		delete mBackground;
	}

	mBackground = bkgnd;
}

//=============================================================================
//      QGScene::AddObject : Add an object to the scene - 
//			NB: has side effect of removing the object if it's already inserted
//-----------------------------------------------------------------------------
void QGScene::AddObject( QGObject* obj )
{
	if (obj->mScene) obj->mScene->RemoveObject(obj);
	mObjects.push_back(obj);
	obj->mScene = this;
}

//=============================================================================
//      QGScene::SetBackground : Set the background object - used for invariant backgrounds
//-----------------------------------------------------------------------------
void QGScene::RemoveObject( QGObject* obj )
{
	VecReverseIterate(i, mObjects) {
		if (mObjects[i] == obj) {
			mObjects.deleteIdx(i);
			obj->mScene = NULL;
			return;
		}
	}
	#ifdef DEBUG
		DEBUGSTR("\pRemoveObject called, but object not found!");
	#endif
}

//=============================================================================
//      QGScene::CollisionCheck : ask objects to check for collisions
//-----------------------------------------------------------------------------
void QGScene::CollisionCheck(float dt)
{
	// For now, let's just check for new collisions...
	short first = 0, last = mObjects.size()-1;
	VecReverseIterate(i, mObjects) {
		if (first < last) mObjects[i]->NewCollisions( dt, mObjects, first, last );
	}
}

//=============================================================================
//      QGScene::MoveObjects : move objects by their velocities
//-----------------------------------------------------------------------------
void QGScene::MoveObjects(float dt)
{
	VecReverseIterate(i, mObjects) {
		TQ3Vector3D v = mObjects[i]->GetVelocity();
		if (v.x != 0.0f or v.y != 0.0f or v.z != 0.0f) {
			mObjects[i]->Move(v.x*dt, v.y*dt, v.z*dt);
		}
	}
}

//=============================================================================
//      QGScene::PrepareToRender : Inform objects we're about to render
//-----------------------------------------------------------------------------
void QGScene::PrepareToRender()
{
	// Prepare for rendering (make sure bounding boxes are up-to-date, etc.).
	VecIterate(i, mObjects) {
		mObjects[i]->PrepareToRender();
	}
	
	if (mBackground) {
		mBackground->PrepareToRender(kQ3False);	// (don't try to cull the background!)
	}

}

//=============================================================================
//      QGScene::Render : submit styles, then background, then shader, then objects
//-----------------------------------------------------------------------------
void QGScene::Render()
{
	// Make sure we're initialized.
	if (!mView) return;

	// submit rendering prefs
	Q3Style_Submit( mInterpolation, mView );
	Q3Style_Submit( mBackFacing, mView );
	Q3Style_Submit( mFillStyle, mView );
	
	// submit the background (do this *before* the shader)
	if (mBackground) {
		if (mCamera and mCamera->mCamChanged) {
			mBackground->MoveTo( mCamera->GetPosition() );
		}
		mBackground->Submit( mView );
	}
	
	// submit any other pre-shader extras
	PreshaderSubmit();
	
	if (mShader) Q3Shader_Submit( mShader, mView );

	// OFI: be smarter about which models to submit, and in what order!
	VecIterate(i, mObjects) {
		mObjects[i]->Submit();
	}
	
	// submit any other post-shader extras
	PostshaderSubmit();

	// reset the camera-changed flag
	if (mCamera) mCamera->mCamChanged = kQ3False;
}

//=============================================================================
//      QGScene::LineIntersectsObject : returns whether a given line 
//			intersects an object in the scene.
//			Return value is the *closest* object hit (if any),
//			and outPoint contains the point hit.
//-----------------------------------------------------------------------------
QGObject *QGScene::LineIntersectsObject(const short collisionMask,
										 const TQ3Point3D& p1,const TQ3Point3D& p2, // line to test
										 TQ3Point3D *hitPoint) // intersection point
{
	QGObject	*hitObject			= nil;
	float		hitObjectDistance 	= kQ3MaxFloat;
	
	// Iterate through all our objects asking them if they're hit by this line.
	// We keep going until we finish so we find the closest object.
	// (May want to do a routine which returns all objects hit at some point.)
	
	VecIterate(i, mObjects) {
		if (mObjects[i]->LineIntersectsObject(collisionMask,p1,p2,hitPoint)) {
				float tempHitObjectDistance = Q3Point3D_DistanceSquared(&p1,hitPoint);
				if (tempHitObjectDistance < hitObjectDistance) {
						hitObjectDistance = tempHitObjectDistance;
						hitObject = mObjects[i];
						}
			}
	}
	
	return hitObject;
}


