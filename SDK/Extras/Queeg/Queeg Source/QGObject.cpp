
/*  NAME:
        QGObject.cpp
        
    DESCRIPTION:
		Object class which keeps track of a collection of QGTriMeshes
		and provides routines for movement and collision detection

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
#include "QGObject.h"
#include "QGTriMesh.h"
#include "QGScene.h"
#include "QueegMacros.h"

#include "QuesaCamera.h"
#include "QuesaStorage.h"
#include "QuesaGroup.h"
#include "QuesaIO.h"





//=============================================================================
//      QGObject::QGObject : Create an empty 3D object at the origin.
//-----------------------------------------------------------------------------
QGObject::QGObject()
: mScene(NULL), mBoundsDirty(0), mSqrRadius(0.0f),
  mCollMethod(kSphereMethod), mCollisionGroup(0), mCollisionMask(0),
  mDying(false), mVisible(true)
{
	Q3Point3D_Set( &mPosition, 0.0f, 0.0f, 0.0f );
	Q3Vector3D_Set( &mVelocity, 0.0f, 0.0f, 0.0f );
	Q3Quaternion_SetRotate_XYZ( &mOrientation, 0.0f, 0.0f, 0.0f );
}

//=============================================================================
//      QGObject::~QGObject : Destroy an object and all its contents.
//-----------------------------------------------------------------------------
QGObject::~QGObject()
{
//	if (mScene) mScene->RemoveObject(this);
	VecReverseIterate(i, mTriMeshes) {
		delete mTriMeshes[i];
	}
}

//=============================================================================
//      QGObject::Clone : Create a new object identical to this one.
//-----------------------------------------------------------------------------
QGObject* QGObject::Clone(QGObject* newobj)
{
	// create a new object, identical to this one
	// Or, if passed an (empty) object, just make it match 
	QGObject *out;
	if (newobj) out = newobj;
	else {
		out = new QGObject;
		if (not out) {
			DEBUGSTR("\pMemory allocation failed in Clone");
			return NULL;
		}
	}
	out->mPosition = mPosition;
	out->mOrientation = mOrientation;
	out->mBounds = mBounds;
	out->mBoundsDirty = mBoundsDirty;
	out->mSqrRadius = mSqrRadius;
	out->mCollisionGroup = mCollisionGroup;
	out->mCollisionMask = mCollisionMask;
	out->mCollMethod = mCollMethod;
	// (no need to copy mVisible -- it's recalculated at render time)

	// copy trimeshes as well
	ASSERT(!out->mTriMeshes.size());	// newobj shouldn't already have trimeshes!
	VecIterate(i, mTriMeshes) {
		QGTriMesh *t = new QGTriMesh(*mTriMeshes[i]);
		if (not t) {
			DEBUGSTR("\pMemory allocation failed in Clone");
			delete out;
			return NULL;
		}
		out->mTriMeshes.push_back( t );
	}
//	if (mScene) {
//		mScene->AddObject(out);
//	} else 
		out->mScene = 0;

	return out;
}

//=============================================================================
//      QGObject::AddMesh : Add a QGTriMesh to this object.
//-----------------------------------------------------------------------------
void QGObject::AddMesh(QGTriMesh* mesh)
{
	// OFI: keep these semi-sorted, such that biggest mesh comes first
	mTriMeshes.push_back(mesh);
	mBoundsDirty = true;
}

//=============================================================================
//      QGObject::AddFrom3DMF : Add TriMeshes found in a file (accessed
//			by name in an ANSI-standard way).
//-----------------------------------------------------------------------------
short QGObject::AddFrom3DMF(const char* filename)
{
	// open the file
	FILE *file = fopen( filename, "rb" );
	if (file == NULL) return 0;
	
	// get the file size
	fseek(file, 0, SEEK_END);
	SInt32 eof = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	// allocate space and read into memory
	short qtyAdded = 0;
	unsigned char *buffer = new unsigned char[eof];
	if (buffer) {
		fread( buffer, 1, eof, file );
		// pass to the workhorse function
		qtyAdded = AddFrom3DMF( buffer, eof );
		delete[] buffer;
	} else {
		DEBUGSTR("\pCouldn't allocate buffer in AddFrom3DMF");
	}
	
	fclose(file);
	return qtyAdded;
}

//=============================================================================
//      QGObject::AddFrom3DMF : Add TriMeshes found in a 3DMF buffer.
//-----------------------------------------------------------------------------
short QGObject::AddFrom3DMF(unsigned char* buffer, TQ3Uns32 bufsize)
{
	// Add objects from the given buffer, which must contain 3DMF data.
	// Return count of objects added.
	
	TQ3StorageObject	storageObj;
	TQ3FileObject		fileObj;
	short qtyAdded = 0;
			
	// create a storage object corresponding to the 3DMF file
	storageObj = Q3MemoryStorage_NewBuffer(buffer, bufsize, bufsize);
	if (not storageObj) {
		DEBUGSTR("\pError making storage buffer in AddFrom3DMF");
		return qtyAdded;
	}

	// create a file object
	fileObj = Q3File_New();
	if (not fileObj) return qtyAdded;

	// set the storage to the file
	TQ3Status err = Q3File_SetStorage(fileObj, storageObj);
	if (err != kQ3Success) {
		DEBUGSTR("\pError setting storage in AddFrom3DMF");
		// NOTE: this leaks, but should never happen in a shipping app anyway.
		return qtyAdded;
	}
	
	// dispose of the storage object -- no longer needed
	Q3Object_Dispose(storageObj);
	
	// Now we have a file object for the 3DMF file.

	// Open the file object, and examine each object in turn.
	err = Q3File_OpenRead(fileObj,nil);
	if (err != kQ3Success) return qtyAdded;
	do {
		// read a metafile object
		// (does anyone else notice that "metafile" is a misnomer here?!?)
		TQ3Object obj = Q3File_ReadObject(fileObj);
		if (!obj) {
			// this should never happen -- we'll get an end-of-file first
			// so treat this as a semi-serious problem
			DEBUGSTR("\pend-of-file error in AddFrom3DMF");
		}
		
		// add any trimeshes it contains to our output object
		qtyAdded = AddQD3DObject( obj );
		
		// done with this object, trash it
		Q3Object_Dispose(obj);
	
	} while (!Q3File_IsEndOfFile(fileObj));
	
	// dispose of file and storage objects and return
	Q3Object_Dispose(fileObj);
	return qtyAdded;
}

//=============================================================================
//      QGObject::AddQD3DObject : Examine the given Quesa object, and if any
//			trimeshes are found, add 'em to this QGObject.  Return the count
//			of trimeshes added.  This function calls itself recursively to
//			search groups.
//-----------------------------------------------------------------------------
short QGObject::AddQD3DObject( const TQ3Object qd3dObj )
{
	short qtyAdded = 0;
	
	// see if the object is a QGTriMesh
	// (That's all we care about at this point...
	//  Later, we may support arbitrary drawable QD3D objects)
	if (Q3Object_IsType( qd3dObj, kQ3GeometryTypeTriMesh )) {
		// yep, it's a QD3D trimesh...
		// now, copy its data into an M3D QGTriMesh object
		QGTriMesh *mesh;
		mesh = new QGTriMesh((TQ3GeometryObject)qd3dObj);
		if (not mesh) return qtyAdded;
		AddMesh(mesh);
		qtyAdded++;
	}
	else if (Q3Object_IsType( qd3dObj, kQ3ShapeTypeGroup )) {
		// it's a group object... now we gotta examine each member
		// of the group, and look for any trimeshes lurking below
		TQ3GroupPosition pos;
		for (Q3Group_GetFirstPosition(qd3dObj, &pos); pos; Q3Group_GetNextPosition(qd3dObj, &pos)) {
			TQ3Object subObj;
			Q3Group_GetPositionObject(qd3dObj, pos, &subObj);
			qtyAdded += AddQD3DObject( subObj );
			Q3Object_Dispose(subObj);
		}
	}

	return qtyAdded;
}

//=============================================================================
//      QGObject::AddQD3DObject : Find the greatest squared distance between
//			any point in any mesh, and the object's position.  This becomes
//			the new mSqrRadius.
//-----------------------------------------------------------------------------
void QGObject::ResetRadius()
{
	// find the maximum squared distance from any point in any mesh to mPosition
	TQ3Point3D center = {mPosition.x, mPosition.y, mPosition.z};
	float maxd = 0;
	VecIterate(meshno, mTriMeshes) {
		TQ3Point3D* pts = mTriMeshes[meshno]->Points();
		short qtyPts = mTriMeshes[meshno]->QtyPoints();
		for (TQ3Uns32 i=0; i<qtyPts; i++) {
			float d = Q3Point3D_DistanceSquared( &pts[i], &center );
			if (d > maxd) maxd = d;
		}
	}
	mSqrRadius = maxd;
}


//=============================================================================
//      QGObject::NewCollisions : Check for collisions that might happen in
//			the next dt time steps, based on the velocity of this object
//			and a list of other objects to consider.
//			NOTE: the velocity of the *other* objects is ignored.
//-----------------------------------------------------------------------------
void QGObject::NewCollisions(float dt, SimpleVector<QGObject*> mObjects, short firstIdx, short lastIdx)
{
	// Check for collisions that might happen in the 
	//
	
	// (If we're not moving, or if it's not possible for us to 
	// collide with anything, return immediately!
	if ((!mCollisionGroup and !mCollisionMask) or
		(mCollMethod == kSphereMethod and !mSqrRadius) or
		(!mVelocity.x and !mVelocity.y and !mVelocity.z)) return;		

	// we want to be careful not to miss anything here....
	// as a Pretty Good Heuristic, we'll check along our path every
	// radius
	
	float len = Q3Vector3D_Length(&mVelocity) * dt;
	float radius = GetRadius();
	TQ3Point3D oldpos = mPosition;
	float factor=0.0f;
	for (float dist=radius; dist < len and !mDying; dist += radius) {
		factor = dist/len;
		TQ3Vector3D v;
		Q3Vector3D_Scale( &mVelocity, factor, &v );
		Q3Point3D_Vector3D_Add( &oldpos, &v, &mPosition );
		CurrentCollisions( mObjects, firstIdx, lastIdx );
	}
	if (mDying) return;
	
	// then, check at the final position...
	if (factor < 1) {
		TQ3Vector3D v;
		Q3Vector3D_Scale( &mVelocity, dt, &v );
		Q3Point3D_Vector3D_Add( &oldpos, &v, &mPosition );
		CurrentCollisions( mObjects, firstIdx, lastIdx );
	}

	// finally, restore previous position
	// LATER: be careful -- collision-handling routine might change this?
	mPosition = oldpos;
}	
	
//=============================================================================
//      QGObject::CurrentCollisions : Check for collisions currently
//			happening between this object and others which have an
//			appropriate collision mask.  Verify any collisions by calling
//			CollisionVerify on each object, and both objects agree that
//			the collision is genuine, call Collide on each object.
//-----------------------------------------------------------------------------
void QGObject::CurrentCollisions(SimpleVector<QGObject*> mObjects, short firstIdx, short lastIdx)
{
	// if it's not possible for us to collide with anything, return immediately!
	if ((!mCollisionGroup and !mCollisionMask) or
		(mCollMethod == kSphereMethod and !mSqrRadius) or mDying) return;		

	ASSERT( firstIdx >= 0 and lastIdx >= 0 );
	ASSERT( firstIdx <= lastIdx and lastIdx < mObjects.size() );
	
	// check for collisions between us and every object in the given range...
	// OFI: restructure this using a separate block per test, using goto to jump to the next object
	bool useBox = (mCollMethod == kBoxMethod);
	for (TQ3Uns32 i=firstIdx; i<lastIdx; i++) {
		QGObject *other = mObjects[i];
		// first, check the collision masks and other's sphere radius
		if (((mCollisionGroup & other->mCollisionMask) or
			 (other->mCollisionGroup & mCollisionMask)) and other->mSqrRadius and
			 other != this and !other->mDying) {
			// next, see if the spheres overlap
			float sqrdist = (mPosition.x-other->mPosition.x) * (mPosition.x-other->mPosition.x)
						  + (mPosition.y-other->mPosition.y) * (mPosition.y-other->mPosition.y)
						  + (mPosition.z-other->mPosition.z) * (mPosition.z-other->mPosition.z);
			if (sqrdist < mSqrRadius + other->mSqrRadius) {
				// next, if either uses a bounding box method, test that
				if (useBox or other->mCollMethod == kBoxMethod) {
					DEBUGSTR("\pkBoxMethod not yet supported!");
					
				}
				// finally, let the objects verify as they wish
				if (CollisionVerify(*other) and other->CollisionVerify(*this)) {
					// we got a collision!
					Collide(*other);
					other->Collide(*this);
				} // end of: if collision is verified
			} // end of: if spheres overlap
		} // end of: if collision masks permit a collision
	} // end of: loop over all other objects
}


//=============================================================================
//      QGObject::LineIntersectsObject : returns whether a given line 
//			intersects the object, outPoint contains the point hit (if any).
//			should possibly return the triangle hit too.
//-----------------------------------------------------------------------------
TQ3Boolean QGObject::LineIntersectsObject(const short collisionMask,
										  const TQ3Point3D& p1,const TQ3Point3D& p2, //line to test
										  TQ3Point3D *outPoint)//intersection point
{
	TQ3Boolean hit = kQ3False;
	
	//check collision group/bits first then check for hit if appropriate
	if (mCollisionGroup & collisionMask){
	
		//should check for hit on bounding sphere/box first 
		//then proceed to check for mesh hit...
	 
	 
		//iterate through meshes checking for hit
		VecIterate(i, mTriMeshes) {
			hit = mTriMeshes[i]->LineIntersectsMesh(p1,p2,outPoint);
			if (hit)
				break;
		}
	}
	
	
	return hit;
}



//=============================================================================
//      QGObject::PrepareToRender : Called just before the object is
//			submitted in a rendering loop.  Updates the bounding box if
//			needed.  LATER: will also handle primitive object culling.
//-----------------------------------------------------------------------------
void QGObject::PrepareToRender(bool cull)
{
	#pragma unused(cull)
	
	if (mBoundsDirty) UpdateBounds();	
/*
	// if our bounding box hasn't changed, and the camera hasn't changed,
	// then our visibility hasn't changed either!
	ASSERT(mScene);		// (can't prepare without a scene)
	else if (!mScene->CamChanged()) return;
	
	// see if we're in the viewing frustum, unless caller specified no culling
	if (!cull) return;

	TQ3Matrix4x4 worldToFrustum;
	TQ3CameraObject camera;
	TQ3ViewObject view = mScene->View();
	Q3View_GetCamera( view, &camera );
	Q3Camera_GetWorldToFrustum( camera, &worldToFrustum );
	Q3Object_Dispose( camera );
	// NOTE: we don't do viewing transformations -- that is,
	//	local coordinates are the same as world coordinates for us.
	//	Thus, we just need the worldToFrustum transformation.

	TQ3Point3D temp, vmin;
	Q3Point3D_Transform( &mBounds.min, &worldToFrustum, &vmin );
	TQ3Point3D vmax = vmin;
	Q3Point3D_Transform( &mBounds.max, &worldToFrustum, &temp );
	if (temp.x < vmin.x) vmin.x = temp.x;
	else if (temp.x > vmax.x) vmax.x = temp.x;
	if (temp.z < vmin.z) vmin.z = temp.z;
	else if (temp.z > vmax.z) vmax.z = temp.z;
	TQ3Point3D p = mBounds.min;
	p.z = mBounds.max.z;
	Q3Point3D_Transform( &p, &worldToFrustum, &temp );
	if (temp.x < vmin.x) vmin.x = temp.x;
	else if (temp.x > vmax.x) vmax.x = temp.x;
	if (temp.z < vmin.z) vmin.z = temp.z;
	else if (temp.z > vmax.z) vmax.z = temp.z;
	p.x = mBounds.max.x;
	p.z = mBounds.min.z;
	Q3Point3D_Transform( &p, &worldToFrustum, &temp );
	if (temp.x < vmin.x) vmin.x = temp.x;
	else if (temp.x > vmax.x) vmax.x = temp.x;
	if (temp.z < vmin.z) vmin.z = temp.z;
	else if (temp.z > vmax.z) vmax.z = temp.z;
	
	mVisible = (vmin.x < 1 and vmax.x > -1 and vmin.z < 0 and vmax.z > -1);
*/
}

//=============================================================================
//      QGObject::Submit : Submits the object for rendering in the scene
//			to which it attached.  Skips the submission if mVisible is false.
//-----------------------------------------------------------------------------
void QGObject::Submit()
{
	// Submit this object to the view for rendering.
	// (This should only be called within a rendering loop.)

	// BUT: don't submit if this object is not visible!
	ASSERT(!mBoundsDirty);
	ASSERT( mScene );
	
	if (mVisible) {
		VecIterate(i, mTriMeshes) {
			ASSERT(mTriMeshes[i]);
			mTriMeshes[i]->Submit(mScene->View());
		}
	}
}

//=============================================================================
//      QGObject::Submit : Submits the object for rendering in the given view.
//-----------------------------------------------------------------------------
void QGObject::Submit(TQ3ViewObject &view)
{
	// Submit this object to the view for rendering.
	// (This should only be called within a rendering loop.)

	// Since we don't know anything about the scene --
	// such as whether the camera has moved -- we must
	// assume we're visible, and just submit the trimeshes.

	ASSERT(!mBoundsDirty);
	
	VecIterate(i, mTriMeshes) {
		ASSERT(mTriMeshes[i]);
		mTriMeshes[i]->Submit(view);
	}
}

//=============================================================================
//      QGObject::Move : Shifts the object's position in space.
//-----------------------------------------------------------------------------
void QGObject::Move(const float dx, const float dy, const float dz)
{
	// update all triangle vertices...
	// since this is mere translation, no need to update normals etc.
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->Move( dx, dy, dz );
	}
	mPosition.x += dx;
	mPosition.y += dy;
	mPosition.z += dz;

	// also update the bounding box (unless it's already dirty)
	if (!mBoundsDirty) {
		mBounds.min.x += dx;
		mBounds.min.y += dy;
		mBounds.min.z += dz;
		mBounds.max.x += dx;
		mBounds.max.y += dy;
		mBounds.max.z += dz;
	}
	
	// also update the interest points, if any
	VecIterate(i2, mInterestPoints) {
		mInterestPoints[i2].x += dx;
		mInterestPoints[i2].y += dy;
		mInterestPoints[i2].z += dz;
	}
}

//=============================================================================
//      QGObject::MoveForward : Moves the object "forward" a given distance.
//			Forward is down the Z axis initially; as the object is rotated,
//			the "forward" axis is adjusted accordingly.
//-----------------------------------------------------------------------------
void QGObject::MoveForward(const float distance)
{
	// Find forward vector.
	TQ3Vector3D forward = GetForwardVector(distance);

	// Then, move by this delta.
	Move(forward.x, forward.y, forward.z);
}

//=============================================================================
//      QGObject::Orient : Apply a rotation (defined by a quaternion)
//			to the object.
//-----------------------------------------------------------------------------
void QGObject::Orient(const TQ3Quaternion& q)
{
	// add rotation q to our current rotation
	TQ3Quaternion newq;
	Q3Quaternion_Multiply( &mOrientation, &q, &newq );
	OrientTo(newq);
}

//=============================================================================
//      QGObject::Orient : Apply a rotation (defined by Euler angles)
//			to the object.  Rotations are applied in X, Y, Z order,
//			following the QD3D standard.
//-----------------------------------------------------------------------------
void QGObject::Orient(float dXang, float dYang, float dZang)
{
	// adjust orientation by the given Euler angles
	// (applied in QD3D order, i.e. Xang, Yang, Zang)
	TQ3Quaternion q;
	Q3Quaternion_SetRotate_XYZ( &q, dXang, dYang, dZang );
	Q3Quaternion_Multiply( &mOrientation, &q, &q );
	OrientTo( q );
}

//=============================================================================
//      QGObject::OrientTo : Reorient the object to match the given quaternion.
//-----------------------------------------------------------------------------
void QGObject::OrientTo(const TQ3Quaternion& q)
{
	// Reorient the object to match quaternion q.
	
	if (not mTriMeshes.size()) return;	// don't bother if no meshes!
	
	// First, compute a transform that undoes the current translation and rotation...
	TQ3Matrix4x4 transform, tempM;
	TQ3Quaternion tempQ;
	Q3Matrix4x4_SetTranslate( &transform, -mPosition.x, -mPosition.y, -mPosition.z );
	Q3Quaternion_Invert( &mOrientation, &tempQ );
	Q3Matrix4x4_SetQuaternion( &tempM, &tempQ );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );
	
	// then does the new rotation...
	// (be sure to normalize the given quaternion, to avoid accumulation of errors)
	mOrientation = *Q3Quaternion_Normalize( &q, &mOrientation );
	Q3Matrix4x4_SetQuaternion( &tempM, &mOrientation );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );
	
	// then redoes the translation
	Q3Matrix4x4_SetTranslate( &tempM, mPosition.x, mPosition.y, mPosition.z );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );

	// Now we have the complete transformation.
	// Apply this transformation to each mesh in the object.
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->Transform( transform );
	}
	// ...and also to the interest points, if any
	VecIterate(j, mInterestPoints) {
		Q3Point3D_Transform( &mInterestPoints[j], &transform, &mInterestPoints[j] );
	}
	
	// finally, note that bounding box needs to be recomputed
	mBoundsDirty = true;
}

//=============================================================================
//      QGObject::OrientTo : Reorient the object to match the given Euler
//			angles.  These angles are considered in X, Y, Z order, following
//			the QD3D standard.
//-----------------------------------------------------------------------------
void QGObject::OrientTo(float Xang, float Yang, float Zang)
{
	// Orient to the given Euler angles
	// (applied in QD3D order, i.e. Xang, Yang, Zang)
	TQ3Quaternion q;
	Q3Quaternion_SetRotate_XYZ( &q, Xang, Yang, Zang );
	OrientTo( q );
}

//=============================================================================
//      QGObject::Turn : Rotate around one of the *object's* primary axes,
//			i.e., yaw (axis=Y), pitch (axis=X), or roll (axis=Z).  Just
//			a convenience function; it's faster to just call YawPitchRoll.
//-----------------------------------------------------------------------------
void QGObject::Turn(const float radians, const short axis)
{
	switch (axis) {
		case 0: YawPitchRoll( 0, radians, 0 ); break;
		case 1: YawPitchRoll( radians, 0, 0 ); break;
		case 2: YawPitchRoll( 0, 0, radians ); break;
		default:
			#ifdef DEBUG
				DEBUGSTR("\pM3DObject::Turn called with invalid axis parameter");
			#endif
	}
}

//=============================================================================
//      QGObject::YawPitchRoll : Rotate the object around its own primary
//			axes; yaw (rotate about the Y axis), pitch (rotate about X),
//			and roll (rotate about Z), all with respect to the object
//			(regardless of its current orientation).  Multiple rotations
//			are applied in the standard order of yaw, pitch, roll.
//-----------------------------------------------------------------------------
void QGObject::YawPitchRoll(float yaw, float pitch, float roll)
{
	// Rotate relative to object's current orientation.
	// We'll do this by composing quaternions.

	TQ3Quaternion q = mOrientation;	// this will be our new orientation

	// first, get the pitch/yaw/roll axes, and transform them to match the object.
	// then, make a quaternion for each given rotation,
	// and compose with previous rotation
	if (yaw) {
		TQ3Vector3D axis = {0,1,0};
		Q3Vector3D_TransformQuaternion( &axis, &q, &axis );
		TQ3Quaternion temp;
		Q3Quaternion_SetRotateAboutAxis( &temp, &axis, yaw );
		Q3Quaternion_Multiply( &q, &temp, &q );
	}
	
	if (pitch) {
		TQ3Vector3D axis = {1,0,0};
		Q3Vector3D_TransformQuaternion( &axis, &q, &axis );
		TQ3Quaternion temp;
		Q3Quaternion_SetRotateAboutAxis( &temp, &axis, pitch );
		Q3Quaternion_Multiply( &q, &temp, &q );
	}

	if (roll) {
		TQ3Vector3D axis = {0,0,1};
		Q3Vector3D_TransformQuaternion( &axis, &q, &axis );
		TQ3Quaternion temp;
		Q3Quaternion_SetRotateAboutAxis( &temp, &axis, roll );
		Q3Quaternion_Multiply( &q, &temp, &q );
	}
	
	// finally, call the standard OrientTo function to update the object
	OrientTo( q );
}

//=============================================================================
//      QGObject::Scale : Scale the object by a constant factor relative
//			to its position.
//-----------------------------------------------------------------------------
void QGObject::Scale(const float factor)
{
	// undo our translation, then scale, then restore translation
	TQ3Point3D pos = mPosition;
	Move( -pos.x, -pos.y, -pos.z);
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->Scale( factor );
	}
	VecIterate(j, mInterestPoints) {
		mInterestPoints[j].x *= factor;
		mInterestPoints[j].y *= factor;
		mInterestPoints[j].z *= factor;
	}
	Move( pos.x, pos.y, pos.z);
	
	// note that the bounding box needs recomputed
	// (OFI: just scale it here!)
	mBoundsDirty = true;

	// also update the collision radius
	// (OFI: is there a faster way to do this?)
	float r = sqrt(mSqrRadius) * factor;
	mSqrRadius = r*r;
}

//=============================================================================
//      QGObject::Scale : Scale the object relative to its position, with
//			separate scale factors in X, Y, and Z.
//-----------------------------------------------------------------------------
void QGObject::Scale(const float scaleX, const float scaleY, const float scaleZ)
{
	// undo our translation, then scale, then restore translation
	TQ3Point3D pos = mPosition;
	Move( -pos.x, -pos.y, -pos.z);
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->Scale( scaleX, scaleY, scaleZ );
	}
	VecIterate(j, mInterestPoints) {
		mInterestPoints[j].x *= scaleX;
		mInterestPoints[j].y *= scaleY;
		mInterestPoints[j].z *= scaleZ;
	}
	Move( pos.x, pos.y, pos.z);
	
	// note that the bounding box needs recomputed
	// (OFI: just scale it here!)
	mBoundsDirty = true;

	// NOTE: our collision radius is probably now invalid, but there's
	// no perfect way to update it here -- there is no direct relation between
	// non-uniform scaling and the maximum point.  We'll fudge it by scaling
	// the radius by the average scaling factor, which will be good enough
	// in many cases, but users may wish to recompute or manually set the
	// radius after calling this function.

	// If we kept the index of the farthest mesh and point, *then* we could
	// properly scale in this case.  But that would add to the data we have
	// to store, and would be rarely needed -- if this function is ever used,
	// it's probably when the object is initialized anyway.
	
	float factor = (scaleX * scaleY * scaleZ) / 3;
	float r = sqrt(mSqrRadius) * factor;
	mSqrRadius = r*r;
}

//=============================================================================
//      QGObject::FlipSurface : Turn the object inside-out, making all
//			the back surfaces front and vice versa.
//-----------------------------------------------------------------------------
void QGObject::FlipSurface()
{
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->FlipSurface();
	}
}

//=============================================================================
//      QGObject::UpdateBounds : Find (and store) the bounding box which
//			encloses all the object's trimeshes.
//-----------------------------------------------------------------------------
void QGObject::UpdateBounds()
{
	mBounds.isEmpty = kQ3True;		// empty out our current bounding box
	// now union all the trimesh bounds together
	VecIterate(i, mTriMeshes) {
		mTriMeshes[i]->DoneWithChanges();	// make sure trimesh is up-to-date
		Q3BoundingBox_Union( &mBounds, &mTriMeshes[i]->Bounds(), &mBounds );
	}
	mBoundsDirty = false;
}
