/*  NAME:
        Queeg Demo.c
        
    DESCRIPTION:
		A basic test of the Queeg (game utility) classes.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"
#include "QGCamera.h"
#include "QGScene.h"
#include "QGDemoInput.h"

//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const TQ3ColorARGB 	kColorARGBBackground 		= {1.0f, 0.0f, 0.0f, 0.1f};
const float			kObjectHeight				= 2.0f;

const unsigned long kTargetGroup = 0x0001;		// things we can shoot
const unsigned long kGroundGroup = 0x0002;		// things that can support us

//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
QGObject	*gObject	= NULL;
QGScene		*gScene		= NULL;
unsigned short gWhichCam = 2;



//=============================================================================
//      Internal classes & functions
//-----------------------------------------------------------------------------


void Fall(TQ3Boolean	matchSlope);	// Neater if we had header?



//=============================================================================
//		CreateScene : creates a Scene object and some objects to populate it
//-----------------------------------------------------------------------------
static QGScene *CreateScene(TQ3ViewObject theView)
{
	QGScene *scene = new QGScene;
	scene->SetView( theView );

	gObject = new QGObject;
	// NOTE: Quesa 0.0.10 only loads *binary* 3DMF, not text 3DMF
	gObject->AddFrom3DMF("racer.3dm");
	gObject->Scale(0.01);					// model is too big for our view -- scale it down!

	// add a couple of "interest points," which we'll use for camera tracking
	TQ3Point3D p0 = {0.0f, 0.5f, +3.0f};	// where it looks
	TQ3Point3D p1 = {0.0f, 0.5f, +0.0f};	// head cam location
	TQ3Point3D p2 = {0.0f, 1.2f, -5.0f};	// tail cam location
	gObject->mInterestPoints.push_back( p0 );
	gObject->mInterestPoints.push_back( p1 );
	gObject->mInterestPoints.push_back( p2 );

	//add two point of interests for the object fore and aft
	TQ3Point3D p3 = {0.0f,  1.0f, 1.0f};	// head
	TQ3Point3D p4 = {0.0f,  1.0f, -1.0f};	// tail
	gObject->mInterestPoints.push_back( p3 );
	gObject->mInterestPoints.push_back( p4 );

	gObject->Move( 0.0f, 1.0f, 0.0f );		// finally, move the racer up a bit	


	// add the object to the scene
	scene->AddObject( gObject );

	// also, let's add the actual scenery...
	QGObject *map = new QGObject;
	map->AddFrom3DMF("map.3dm");
	map->Scale(0.3);
	map->SetCollisionGroup( kGroundGroup );
	scene->AddObject( map );
	
	// ...and the background...
	QGObject *background = new QGObject;
	background->AddFrom3DMF("background.3dm");
	scene->SetBackground( background );
	
	
	
	// also, let's add the little triangle target for testing rays...
	QGObject *target = new QGObject;
	target->AddFrom3DMF("target.3dm");
	target->Scale(0.03);
	target->MoveTo(0.5f,1.0f,2.0f);
	target->SetCollisionGroup( kTargetGroup );
	scene->AddObject( target );

	
	
	// and prepare the camera...
	scene->Camera()->SetHitherYon( 2.0f, 80.0f );
	scene->Camera()->MoveTo( gObject->mInterestPoints[gWhichCam] );
	scene->Camera()->SetPointOfInterest( gObject->mInterestPoints[0] );

	return scene;
}

//=============================================================================
//		Update : update the scene based on user input, time, etc.
//-----------------------------------------------------------------------------
static void Update()
{
	// find the time step
	// HACK: for now, just do a constant time step per frame
	float dt = 0.1f;
	
	// move all objects
	gScene->MoveObjects(dt);

	// update the racer -- turn or accelerate according to keys pressed
	float dang = 0.05f;
	if (QGKeyIsPressed(keyMoveW)) {
		gObject->YawPitchRoll( dang, 0.0f, 0.0f );
	} else if (QGKeyIsPressed(keyMoveE)) {
		gObject->YawPitchRoll( -dang, 0.0f, 0.0f );
	}

	TQ3Vector3D v = gObject->GetVelocity();	// velocity
	TQ3Vector3D a = gObject->GetForwardVector(5.0f * dt);	// engine acceleration
	if (QGKeyIsPressed(keyMoveN)) {
		gObject->SetVelocity( v.x + a.x, v.y + a.y, v.z + a.z );
	} else if (QGKeyIsPressed(keyMoveS)) {
		gObject->SetVelocity( v.x - a.x, v.y - a.y, v.z - a.z );
	}
	
		
	//to remove - test of triangle hit code
	
	if (QGKeyIsPressed(keyButton0)) {
			
		TQ3Point3D	p1,p2,result;
		p1 = gObject->mInterestPoints[3];
		p2 = p1;
		TQ3Vector3D 	infinity = {0,0,100.0f};//near enough for this kludge
		
		TQ3Quaternion	orientation = gObject->GetOrientation();
		Q3Vector3D_TransformQuaternion( &infinity, &orientation, &infinity );
		
		Q3Point3D_Vector3D_Add( &p2, &infinity, &p2 );
		
		
		QGObject	*hitObject 	= nil;
		
		hitObject = gScene->LineIntersectsObject(kTargetGroup, p1, p2, &result);
		
		if (hitObject)
			hitObject->MoveForward(0.2f);
	
	}
	
	
	
	
	
	
	//make the racer sit a certain distance above the objects below
	Fall(kQ3True);


	// make the camera track the racer (using interest points)
	gScene->Camera()->MoveTo( gObject->mInterestPoints[gWhichCam] );
	gScene->Camera()->SetPointOfInterest( gObject->mInterestPoints[0] );
	

}



//=============================================================================
//     Fall : apply acceleration due to gravity to gObject
//		
//		ASSUMES: that points of interest 3 & 4 are fore and aft of object
//		Note: also applies friction (constant deceleration factor).
//-----------------------------------------------------------------------------
void Fall(TQ3Boolean	/*matchSlope*/)
{	
	
	TQ3Point3D		fore 	= gObject->mInterestPoints[3],
					aft  	= gObject->mInterestPoints[4],
					belowGround, foreGroundHit, aftGroundHit;
	float			foreHeight, aftHeight;
	QGObject		*hitGround 	= nil;
	
	
	//find distance to ground at fore of craft
	TQ3Vector3D 	downVector = {0.0f,kObjectHeight * -10.0f,0.0f};

	Q3Point3D_Vector3D_Add( &fore, &downVector, &belowGround );
	hitGround = gScene->LineIntersectsObject(kGroundGroup, fore, belowGround, &foreGroundHit);

	if (hitGround) foreHeight = fore.y - foreGroundHit.y;
	else foreHeight = fore.y;
	
	float dForeHeight = kObjectHeight - foreHeight;
	
	TQ3Vector3D v = gObject->GetVelocity();
	if (dForeHeight > 0.01f) {
		// apply an upward acceleration due to gravity
		if (dForeHeight > 0.5f) dForeHeight = 0.5f;
		gObject->SetVelocity(v.x*0.9, v.y*0.9 + dForeHeight, v.z*0.9);
	} else if (dForeHeight < -0.01f) {
		// apply a downward acceleration due to gravity
		if (dForeHeight < -0.1f) dForeHeight = -0.1f;
		gObject->SetVelocity(v.x*0.9, v.y*0.9 + dForeHeight, v.z*0.9);
	}
/*
	if (not hitGround) return;
	
	// repeat at back of craft
	Q3Point3D_Vector3D_Add( &aft, &downVector, &belowGround );
	hitGround = gScene->LineIntersectsObject(kGroundGroup, aft, belowGround, &aftGroundHit);
	if (not hitGround) return;	// HACK...
	
	if (hitGround) aftHeight = aft.y - aftGroundHit.y;
	else aftHeight = aft.y;
	
	float dAftHeight = kObjectHeight - aftHeight;
	
	// then, apply a pitch according to difference in fore and aft pressure	
	float pitch =  0.01f * (dAftHeight - dForeHeight);
	if (pitch > 0.1f) pitch = 0.1f;
	else if (pitch < -0.01f) pitch = -0.1f;
	gObject->YawPitchRoll( 0.0f, pitch, 0.0f );
*/
}



#pragma mark - 



//=============================================================================
//      appConfigureView : Configure the view.
//-----------------------------------------------------------------------------
static void
appConfigureView(TQ3ViewObject				theView,
					TQ3DrawContextObject	theDrawContext,
					TQ3CameraObject			theCamera)
{
#pragma unused(theView)
#pragma unused(theCamera)

	// Adjust the background color
	Q3DrawContext_SetClearImageColor(theDrawContext, &kColorARGBBackground);
}



//=============================================================================
//      appMenuSelect : Handle menu selections.
//-----------------------------------------------------------------------------
static void
appMenuSelect(TQ3ViewObject theView, TQ3Uns32 menuItem)
{
	#pragma unused(theView)
	switch (menuItem) {
		case 1:		// head cam
			gWhichCam = 1;
			break;
		case 2:		// tail cam
			gWhichCam = 2;
			break;
	}
}




//=============================================================================
//      appRenderPreFunc : Prepare to render another frame.
//-----------------------------------------------------------------------------
static void
appRenderPreFunc(TQ3ViewObject theView)
{
	// if we don't have a scene, create one now
	if (!gScene) gScene = CreateScene(theView);

	// Update the objects
	Update();

	// prepare the scene
	gScene->PrepareToRender();
}



//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{
	// if we don't have a scene, create one now
	if (!gScene) gScene = CreateScene(theView);

	// render the scene
	ASSERT( gScene->View() == theView );
	gScene->Render();
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{
	// Initialise Qut
	Qut_CreateWindow("Queeg Demo 1", 512, 384, kQ3True);
	Qut_CreateView(appConfigureView);
	Qut_SetRenderFunc(appRender);
	Qut_SetRenderPreFunc(appRenderPreFunc);
	
	// Set up the menu bar
	Qut_CreateMenu(appMenuSelect);
	Qut_CreateMenuItem(kMenuItemLast, "Head Cam");
	Qut_CreateMenuItem(kMenuItemLast, "Tail Cam");
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{
	// Clean up
	if (gScene) delete gScene;
}
