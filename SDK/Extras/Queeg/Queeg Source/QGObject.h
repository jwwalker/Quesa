/*
	QGObject

	This class encapsulates a 3D game object, which may be comprised of one or
	more TriMeshes.  It provides methods for manipulating them as a group.
*/

#ifndef QGOBJECT_H
#define QGOBJECT_H

#include "Quesa.h"
#include "QuesaCustomElements.h"
#include "QuesaMath.h"


#include "SimpleVector.h"	// an expandable array
//#include "M3DGeom.h"		// TQ3Vector3D and matrix functions
#include <math.h>

class QGTriMesh;			// encapsulation of QD3D QGTriMesh definition

class QGObject
{
  friend class QGScene;
  public:
	// constructor/destructor... note that you must NOT use 
	// assignment-op or copy-constructor on these objects!  Use Clone() instead.
	QGObject();
	virtual ~QGObject();		// destroys its triangles
	
	// duplicate this object and all its triangles
	// NOTE: derived classes must override this, create an object of the
	// derived type, then call this method with the newly created object.
	// Users simply say "newfoo = oldfoo.Clone()".
	virtual QGObject* Clone(QGObject* newobj=0);
	
	// add a mesh to the object -- note that the QGObject takes ownership of the trimesh!
	virtual void AddMesh(QGTriMesh*);

	// add trimeshes from a 3DMF buffer, or from a QD3D (possibly compound) object...
	// return the number of trimeshes added
	virtual short AddFrom3DMF(const char* filename);
	virtual short AddFrom3DMF(unsigned char* buffer, TQ3Uns32 bufsize);
	virtual short AddQD3DObject(const TQ3Object qd3dObj);

	// COLLISION MANAGEMENT
	typedef enum { kSphereMethod, kBoxMethod } CollisionMethod;
	
	CollisionMethod GetCollisionMethod() const { return mCollMethod; }
	void SetCollisionMethod( CollisionMethod method) { mCollMethod = method; }
	
	float GetRadius() const { return sqrt(mSqrRadius); }
	void SetRadius(float newRadius) { mSqrRadius = newRadius*newRadius; }
	void ResetRadius();

	short GetCollisionGroup() const { return mCollisionGroup; }
	void SetCollisionGroup(short grp) { mCollisionGroup = grp; }
	
	short GetCollisionMask() const { return mCollisionMask; }
	void SetCollisionMask(short mask) { mCollisionMask = mask; }

	// check for current collisions (usually, don't override this)
	void CurrentCollisions(SimpleVector<QGObject*>, short firstIdx, short lastIdx);

	// check for collisions about to happen, based on our velocity (usually, don't override this)
	void NewCollisions(float dt, SimpleVector<QGObject*>, short firstIdx, short lastIdx);

	// verify a collision at a finer level (feel free to override)
	virtual bool CollisionVerify(QGObject &) { return true; }

	// handle a collision (derived classes should override this)
	virtual void Collide(QGObject&) {}


	//check for intersect of line with objects shapes
	virtual TQ3Boolean LineIntersectsObject(const short collisionMask,
										   const TQ3Point3D& p1,const TQ3Point3D& p2, //line to test
										   TQ3Point3D *hitPoint);

	// prepare for rendering, then submit to the renderer
	virtual void PrepareToRender(bool cull=true);	// call just before the rendering loop
	virtual void Submit();							// call within the rendering loop
	virtual void Submit(TQ3ViewObject &view);		// alternate, to submit to any view

	// misc. inspectors
	const TQ3Point3D& GetPosition() const
	{ return mPosition; }
	
	const TQ3Quaternion& GetOrientation() const
	{ return mOrientation; }
	
	const TQ3Vector3D& GetVelocity() const
	{ return mVelocity; }
	
	inline const TQ3Vector3D GetForwardVector(float length=1.0f) const;
	
	// movement & manipulation
	virtual void SetVelocity(const TQ3Vector3D &v)
		{ mVelocity = v; }
	virtual void SetVelocity(float vx, float vy, float vz)
		{ Q3Vector3D_Set( &mVelocity, vx,vy,vz); }

	virtual void Move(float dx, float dy, float dz);	// <-- primary override point for movement
	virtual void Move(const TQ3Vector3D &delta)
		{ Move(delta.x, delta.y, delta.z); }
		
	virtual void MoveTo(const TQ3Point3D& newPosition)
		{ Move( newPosition.x - mPosition.x, newPosition.y - mPosition.y, newPosition.z - mPosition.z ); }
	virtual void MoveTo(float x, float y, float z )
		{ Move( x - mPosition.x, y - mPosition.y, z - mPosition.z ); }

	virtual void MoveForward(float distance);
	
	virtual void Orient(const TQ3Quaternion& relativeTurn);
	virtual void Orient(float dXang, float dYang, float dZang);
	virtual void Orient(const TQ3Vector3D& deltaOrientation)
		{ Orient( deltaOrientation.x, deltaOrientation.y, deltaOrientation.z ); }

	virtual void OrientTo(const TQ3Quaternion& absOrientation);		// <-- primary override point for orientation
	virtual void OrientTo(float Xang, float Yang, float Zang);
	virtual void OrientTo(const TQ3Vector3D& newOrientation)
		{ OrientTo( newOrientation.x, newOrientation.y, newOrientation.z ); }
	
	virtual void Turn(float radians, short axis=1);	// axis 0=X, 1=Y, 2=Z
	virtual void YawPitchRoll(float yaw, float pitch, float roll);
	
	virtual void Scale(float factor);
	virtual void Scale(float scaleX, float scaleY, float scaleZ);
	virtual void Scale(const TQ3Vector3D& scales)
		{ Scale( scales.x, scales.y, scales.z ); }
	
	virtual void FlipSurface();	// turns inside-out, i.e., flips all triangle normals
	
	// public flag -- used to indicate that the object should be destroyed ASAP
	bool mDying;

	// public list of "interest points" -- updated whenever the object
	// moves, orients, or is scaled; do whatever you want with these!
	SimpleVector<TQ3Point3D> mInterestPoints;

  protected:
	SimpleVector<QGTriMesh *> mTriMeshes;	// component trimeshes
	QGScene *mScene;						// scene we belong to
	TQ3Point3D mPosition;					// object position
	TQ3Quaternion mOrientation;				// object orientation
 	Boolean mBoundsDirty;					// if true, bounding box needs recalculated
	TQ3BoundingBox mBounds;					// bounding box for entire object
	Boolean mVisible;						// if true, object is in viewing frustum
 
  private:
	// enforce the no-copy rule:
	QGObject(const QGObject&);
	QGObject& operator=(const QGObject&);

	// private methods:
	virtual void UpdateBounds();			// update bounding box

	// private data:
	TQ3Vector3D mVelocity;					// current velocity
	float mSqrRadius;						// square of collision radius around mPosition
	short mCollisionGroup;					// collision group -- compared against...
	short mCollisionMask;					// collision mask
	CollisionMethod mCollMethod;			// collision method (box or sphere)
 };


inline const TQ3Vector3D QGObject::GetForwardVector(float length) const
{
	// By default, "forward" for an object is looking down the Z-axis.
	// So we just take this vector and apply our rotation.
	TQ3Vector3D forward = {0,0,length};

	Q3Vector3D_TransformQuaternion( &forward, &mOrientation, &forward );
	return forward;
}


#endif
