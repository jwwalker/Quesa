//	QGScene
//
//	This class encapsulates a set of objects, a QD3D View, the lights, and the camera.
//	Basically everything you need to set up and render a scene.

#ifndef QGSCENE_H
#define QGSCENE_H

class QGObject;
#include "QGCamera.h"
#include "SimpleVector.h"
#include <QD3DStyle.h>
#include <QD3DGeometry.h>

class QGScene
{
  public:
	// constructor: build a standard scene
	QGScene();
	
	// destructor: delete all the scene elements, INCLUDING THE M3DOBJECTS
	virtual ~QGScene();

	// Set rendering preferences.
	typedef enum IlluminationStyle {
		kPhongIllumination,
		kLambertIllumination,
		kNoIllumination
	} IlluminationStyle;		// not in QD3D for some odd reason!
	void SetPrefs(	TQ3InterpolationStyle interpolation,
					TQ3BackfacingStyle backfacing,
					TQ3FillStyle fillStyle,
					IlluminationStyle illumination );

	// Attach the scene to a view.  Must be done before rendering
	// or calling any of the camera-manipulation methods.
	virtual void SetView( TQ3ViewObject );
	
	// Add objects.  The QGScene assumes ownership of the objects,
	// and destroys them when it dies.
	virtual void AddObject( QGObject* );
	
	// Remove an object, yielding ownership of it.
	virtual void RemoveObject( QGObject* );
	
	// Set the background object (always stays a constant distance from the camera)
	virtual void SetBackground( QGObject* );
	
	// Check for collisions, and call Collide() on objects which collide.
	virtual void CollisionCheck(float dt);

	// Update all objects according to their velocity and the given time step.
	virtual void MoveObjects(float dt);
	
	// Render the image.  Note that this is not a virtual function...
	virtual void PrepareToRender();		// (call this before Render())
	void Render();

	// ...but if you have anything extra to submit before/after the shader,
	// you can override these:
	virtual void PreshaderSubmit() {}
	virtual void PostshaderSubmit() {}

	// Return the first object intersected by the line segment p1, p2.
	virtual QGObject *LineIntersectsObject(const short collisionMask,
										  const TQ3Point3D& p1,const TQ3Point3D& p2, //line to test
										  TQ3Point3D *hitPoint);

	
	// inspectors
	QGCamera*		Camera() { return mCamera; }
	TQ3Boolean		CamChanged() { return mCamera->mCamChanged; }
	const TQ3ViewObject& View()	{ return mView; }
		
  protected:
	TQ3ViewObject	mView;				// view (renderer, etc.)
	QGCamera		*mCamera;			// camera (in QG wrappings)
	
  private:
	TQ3StyleObject	mInterpolation;		// interpolation style used when rendering
	TQ3StyleObject	mBackFacing;		// whether to draw shapes that face away from the camera
	TQ3StyleObject	mFillStyle;			// whether drawn as solid filled object or decomposed to components
	TQ3ShaderObject	mShader;			// shading algorithm to use
	TQ3Boolean			mCamChanged;		// kQ3True if camera's changed since last render
	SimpleVector<QGObject*> mObjects;	// objects in the scene
	QGObject		*mBackground;		// object corresponding to the background
};

#endif
