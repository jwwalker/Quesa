#pragma once
/*  NAME:
        C3DSWriter.h

    DESCRIPTION:
        Header file for C3DSWriter.cp.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    ___________________________________________________________________________
*/

#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaIO.h>
#else
	#include <Quesa.h>
	#include <QuesaGeometry.h>
	#include <QuesaIO.h>
#endif

#include <memory>
#include <iosfwd>

struct X3DSWriterImp;
class CQ3ObjectRef;

/*!
	@class		C3DSWriter
	
	@abstract	C++ class to implement the Quesa class to write 3DS files using the lib3ds.
*/
class C3DSWriter
{
public:
							C3DSWriter( TQ3FFormatBaseData* inData );
							~C3DSWriter();
	
	/*!
		@function			FromFileFormat
		@abstract			Retrieve a C3DSWriter pointer given a file format
							object.	(Static method)
	*/
	static C3DSWriter*		FromFileFormat( TQ3FileFormatObject inFormat );

	/*!
		@function			FromFile
		@abstract			Retrieve a C3DSWriter pointer given a file
							object.	(Static method)
	*/
	static C3DSWriter*		FromFile( TQ3FileObject inFile );
	
	
	/*!
		@function			StartFile
		@abstract			.
		@discussion			.
		@result				.
	*/
	TQ3Status				StartFile(
									TQ3ViewObject				theView,
									TQ3FFormatBaseData			*fileFormatPrivate,
									TQ3DrawContextObject		theDrawContext);
	
	/*!
		@function			EndPass
		@abstract			.
		@result				.
	*/
	TQ3ViewStatus			EndPass(
									TQ3ViewObject				theView,
									TQ3FFormatBaseData		*fileFormatPrivate);
	
	/*!
		@function			Close
		@abstract			The file is being closed.  If there is a debug
							stream, it is finished and returned to the client
							now.
	*/
	TQ3Status				Close( TQ3Boolean inAbort );
	

///////// Geometry management

	/*!
		@function			Write_TriMesh
		@abstract			Trimesh Handler.
	*/
	void					Write_TriMesh( TQ3TriMeshData *geomData );
	
	/*!
		@function			Write_Triangle
		@abstract			Triangle Handler.
	*/
	void					Write_Triangle(TQ3TriangleData *geomData);
	
///////// Transform management

	/*!
		@function			UpdateMatrix
		@abstract			A new Matrix is submitted, so let's update our matrix state.
	*/
	void					UpdateMatrix( TQ3Matrix4x4* theMatrix );
	
	/*!
		@function			PushGroupLevel
		@abstract			Add a frame to our matrix stack.
	*/
	void					PushGroupLevel();
	
	/*!
		@function			PopGroupLevel
		@abstract			Pop a frame from our matrix stack .
	*/
	void					PopGroupLevel();
	
///////// Debug

	/*!
		@function			GetDebugStream
		@abstract			Accessor for a debug text stream to be used by
							node handlers.
		@result				An output stream pointer, or NULL.
	*/
	std::ostream*			GetDebugStream();
	
	

private:
	std::auto_ptr<X3DSWriterImp>		mImp;
	
		
};
