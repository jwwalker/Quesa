/*
	QGCamera
	
	This class encapsulates a view camera.  It is derived from QGObject because
	you might want to do all the same sorts of things with it: move it around,
	compute collisions with it, etc.  Normally you would not add any TriMeshes
	to it (though you can if you want).
*/

#ifndef QGCAMERA_H
#define QGCAMERA_H

#include "QGObject.h"
#include "QuesaCamera.h"

class QGCamera : public QGObject
{
  typedef QGObject inherited;
  public:
	QGCamera(TQ3CameraObject cam);
	
	// METHODS OVERRIDING QGOBJECT METHODS:	
	virtual QGObject* Clone(QGObject* newobj=0);
	virtual void Move(float dx, float dy, float dz);
	virtual void OrientTo(const TQ3Quaternion& absOrientation);	

	// CUSTOM METHODS:
	
	// get/set the point of interest (what the camera's looking at)
	virtual TQ3Point3D& GetPointOfInterest() { return mPlacement.pointOfInterest; }
	virtual void SetPointOfInterest(const TQ3Point3D&);

	// set the hither & yon
	virtual void SetHitherYon( float hither, float yon );

	// get a reference to the camera -- use with caution!
	virtual const TQ3CameraObject GetCam() { return mCamera; }

	// PUBLIC VARIABLES:
	TQ3Boolean mLockPointOfInterest;	// if kQ3True, POI stays put even when camera moves
	TQ3Boolean mCamChanged;				// true when camera is changed; set it to false yourself

  private:
	TQ3CameraObject mCamera;			// Quesa camera object
	TQ3CameraPlacement mPlacement;		// its placement (stored separately for speed)
};

#endif
