/*
 *  TriMesh.h
 *  Textify3DMF
 *
 *  Created by James Walker on 4/7/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */


class TriMesh : public TypeHandler
{
public:
							TriMesh();
	
	virtual void			Process( uint32_t inStartOffset,
									uint32_t inEndOffset );

	void					GetCounts( uint32_t& outFaces,
									uint32_t& outEdges,
									uint32_t& outPoints ) const;

private:
	uint32_t				FetchIndex( uint32_t inStartOffset, int inBytes );
	void					WriteTriangles( uint32_t inStartOffset,
											uint32_t inNumFaces,
											int inBytesPerPointIndex );
	void					WriteEdges( uint32_t inStartOffset,
											uint32_t inNumEdges,
											int inBytesPerPointIndex,
											int inBytesPerFaceIndex );
	void					WritePoints( uint32_t inStartOffset,
											uint32_t inNumPoints );
	void					WriteBoundingBox( uint32_t inStartOffset );
	
	// Characteristings of the TriMesh last seen
	uint32_t				mFaces;
	uint32_t				mEdges;
	uint32_t				mPoints;
};
