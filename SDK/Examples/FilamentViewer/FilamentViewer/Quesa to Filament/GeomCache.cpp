//
//  GeomCache.cpp
//  FilamentViewer
//
//  Created by James Walker on 3/22/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "GeomCache.h"

#import "BoundsToBox.h"
#import "FindTriMeshVertexData.h"
#import "LogToConsole.h"
#import "SynthesizeTriMeshUVs.h"
#import "SynthesizeTriMeshVertexNormals.h"

#import <Quesa/CQ3ObjectRef_Gets.h>
#import <Quesa/CQ3WeakObjectRef.h>
#import <Quesa/QuesaGeometry.h>
#import <Quesa/QuesaMath.h>
#import <Quesa/QuesaMathOperators.hpp>
#import <Quesa/QuesaSet.h>
#import <Quesa/QuesaStyle.h>

#import <geometry/SurfaceOrientation.h>
#import <backend/BufferDescriptor.h>
#import <filament/IndexBuffer.h>
#import <filament/VertexBuffer.h>
#import <filament/RenderableManager.h>
#import <filament/Box.h>
#import <backend/DriverEnums.h>

#import <map>
#import <vector>
#import <assert.h>
#import <unordered_set>

using namespace filament;

namespace
{
	typedef std::vector< filament::math::quatf >	QuatVec;
	typedef std::vector< TQ3Uns32 >					IndexVec;
	typedef std::vector< TQ3Point3D >				PositionVec;
	typedef std::vector< TQ3Param2D >				UVVec;
	typedef std::vector< TQ3ColorRGBA >				ColorVec;
	typedef std::vector< math::quatf >				QuatVec;

	typedef std::map< TQ3GeometryObject, sharedFilaGeom >	GeomCacheMap;
	
	typedef std::pair< TQ3Uns32, TQ3Uns32 >			IndexPair;
	
	struct HashEdge
	{
		size_t	operator()( const IndexPair& inPair ) const noexcept;
	};
	
	typedef std::unordered_set< IndexPair, HashEdge >			EdgeSet;
}

template <typename T>
inline void BufferDescriptorCleanup( void* buffer, size_t size, void* userData )
{
	std::vector< T >* bufVec = reinterpret_cast<std::vector< T >*>( userData );
	delete bufVec;
	LogToConsole("Released buffer storage of size %zu", size );
}

size_t	HashEdge::operator()( const IndexPair& inPair ) const noexcept
{
	static_assert( sizeof(size_t) >= 8, "bad size_t size" );
	static_assert( sizeof(TQ3Uns32) == 4, "bad TQ3Uns32 size" );
	size_t result = (static_cast<size_t>( inPair.first ) << 32) |
		static_cast<size_t>( inPair.second );
	return result;
}


class GeomCacheImp
{
public:
						GeomCacheImp( filament::Engine& inEngine );
	
	/*!
		@function		GetGeom
		
		@abstract		Create, or retrieve from the cache, Filament geometry data for a TriMesh.
		@param			inGeom		A TriMesh.
		@param			inFillStyle	The current fill style.
		@result			A wrapped FilaGeom, or wrapped nil if something went wrong.
	*/
	sharedFilaGeom		GetGeom( TQ3GeometryObject inGeom,
								TQ3FillStyle inFillStyle );
	
	/*!
		@function		Flush
		@abstract		Flush any cached FilaGeom records for which the original naked TriMesh
						no longer exists or has an outdated edit index.
	*/
	void				Flush();

private:
	void				GetGeomTriMesh();
	void				GetGeomNonTriMesh();
	void				GetTriangleEdgeIndices();
	void				GetTrMeshEdgeIndices();
	void				GetTriMeshImplicitEdgeIndices( const TQ3TriMeshData& inTMData );
	void				GetTriMeshExplicitEdgeIndices( const TQ3TriMeshData& inTMData );
	bool				LoadPolyLine();
	bool				LoadLine();
	bool				LoadPoint();
	bool				LoadTriangle();
	void				LoadTriMesh( const TQ3TriMeshData* inTMData );
	
	// Long-term members
	filament::Engine&	_engine;
	GeomCacheMap		_cache;
	
	// Used during one GetGeom call, for the convenience of not passing stuff
	// to a lot of subroutines
	TQ3GeometryObject	_quesaGeom;
	sharedFilaGeom		_filaGeom;
};

static void GatherVertexColors(
				TQ3Uns32 inVertexCount,
				const TQ3ColorRGB* inDiffuseColors,
				const TQ3ColorRGB* inTransparencyColors,
				std::unique_ptr< std::vector<TQ3ColorRGBA> >& outColors )
{
	if ( (inVertexCount > 0) and (inDiffuseColors != nullptr) )
	{
		outColors.reset( new std::vector<TQ3ColorRGBA>( inVertexCount ) );
		
		for (TQ3Uns32 i = 0; i < inVertexCount; ++i)
		{
			TQ3ColorRGBA& dstColor( (*outColors)[i] );
			const TQ3ColorRGB& srcColor( inDiffuseColors[i] );
			dstColor.r = srcColor.r;
			dstColor.g = srcColor.g;
			dstColor.b = srcColor.b;
			if (inTransparencyColors == nullptr)
			{
				dstColor.a = 1.0f;
			}
			else
			{
				const TQ3ColorRGB& srcTrans( inTransparencyColors[i] );
				dstColor.a = (srcTrans.r + srcTrans.g + srcTrans.b) / 3.0f;
			}
		}
	}
}

GeomCacheImp::GeomCacheImp( filament::Engine& inEngine )
	: _engine( inEngine )
{
}

void	GeomCacheImp::LoadTriMesh( const TQ3TriMeshData* inTMData )
{
	// See what data we have to work with, other than positions.
	// In theory, 3DMF or Collada models could specify tangent vectors,
	// but I do not think I have any examples of either.
	std::unique_ptr< std::vector<TQ3Vector3D> >		vertNormals;
	CopyOrSynthesizeVertexNormals( *inTMData, vertNormals );
	
	std::unique_ptr< std::vector<TQ3Param2D> > vertUVs;
	CopyOrSynthesizeUVs( *inTMData, vertUVs );
	
	std::unique_ptr< std::vector<TQ3ColorRGBA> > vertColors;
	const TQ3ColorRGB* diffuseColors = reinterpret_cast<const TQ3ColorRGB*>(
		FindTriMeshVertexData( inTMData, kQ3AttributeTypeDiffuseColor ) );
	const TQ3ColorRGB* transparencyColors = reinterpret_cast<const TQ3ColorRGB*>(
		FindTriMeshVertexData( inTMData, kQ3AttributeTypeTransparencyColor ) );
	GatherVertexColors( inTMData->numPoints, diffuseColors, transparencyColors,
		vertColors );
	
	// Use the data we have to synthesize orientation quaternions
	filament::geometry::SurfaceOrientation::Builder builder;
	builder.vertexCount( inTMData->numPoints )
		.positions( reinterpret_cast<const filament::math::float3*>(
			inTMData->points ) )
		.triangleCount( inTMData->numTriangles )
		.triangles( reinterpret_cast<const filament::math::uint3 *>(
			inTMData->triangles ) )
		.normals( reinterpret_cast<const filament::math::float3 *>(
			vertNormals->data() ) )
		.uvs( reinterpret_cast<const filament::math::float2 *>( vertUVs->data() ) );
	filament::geometry::SurfaceOrientation* orient = builder.build();
	assert( orient != nullptr );
	std::unique_ptr<QuatVec> quats;
	quats.reset( new QuatVec( inTMData->numPoints ) );
	orient->getQuats( &(*quats)[0], inTMData->numPoints );

	uint8_t numVertexBuffers = 3;	// POSITION, UV0, TANGENT
	uint8_t positionBufferIndex = 0;
	uint8_t uvBufferIndex = 1;
	uint8_t	tangentBufferIndex = 2;
	uint8_t colorBufferIndex = 3;
	if (vertColors.get() != nullptr)
	{
		++numVertexBuffers; // COLOR
	}
	
	// Set up the index buffer
	IndexBuffer* theIndexBuf = IndexBuffer::Builder()
		.indexCount( 3 * inTMData->numTriangles )
		.bufferType( IndexBuffer::IndexType::UINT )
		.build( _engine );
	_filaGeom->_indexBufferTriangles.reset( theIndexBuf, &_engine );
	std::unique_ptr< IndexVec > indices( new IndexVec(
		&inTMData->triangles[0].pointIndices[0],
		&inTMData->triangles[inTMData->numTriangles].pointIndices[0] ) );
	IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
		inTMData->numTriangles * sizeof(TQ3TriMeshTriangleData),
		BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
	_filaGeom->_indexBufferTriangles->setBuffer( _engine, std::move(indexDesc) );

	// Set up the vertex buffer
	VertexBuffer::Builder vbBuild;
	vbBuild.vertexCount( inTMData->numPoints )
		.bufferCount( numVertexBuffers )
		.attribute( VertexAttribute::POSITION, positionBufferIndex,
			VertexBuffer::AttributeType::FLOAT3 )
		.attribute( VertexAttribute::UV0, uvBufferIndex,
			VertexBuffer::AttributeType::FLOAT2 )
		.attribute( VertexAttribute::TANGENTS, tangentBufferIndex,
			VertexBuffer::AttributeType::FLOAT4 );
	
	if (vertColors.get() != nullptr)
	{
		vbBuild.attribute( VertexAttribute::COLOR, colorBufferIndex,
			VertexBuffer::AttributeType::FLOAT4 );
	}
	
	_filaGeom->_vb.reset( vbBuild.build( _engine ), &_engine );
	
	// Position data
	std::unique_ptr< PositionVec > positions( new PositionVec(
		&inTMData->points[0], &inTMData->points[inTMData->numPoints] ) );
	VertexBuffer::BufferDescriptor posDesc( &(*positions)[0],
		inTMData->numPoints * sizeof(TQ3Point3D),
		BufferDescriptorCleanup< TQ3Point3D >, positions.release() );
	_filaGeom->_vb->setBufferAt( _engine, positionBufferIndex, std::move(posDesc) );
	
	// UV data
	VertexBuffer::BufferDescriptor uvDesc( &(*vertUVs)[0],
		inTMData->numPoints * sizeof(TQ3Param2D),
		BufferDescriptorCleanup<TQ3Param2D>, vertUVs.release() );
	_filaGeom->_vb->setBufferAt( _engine, uvBufferIndex, std::move(uvDesc) );
	
	// Tangent space data
	VertexBuffer::BufferDescriptor quatDesc( &(*quats)[0],
		inTMData->numPoints * sizeof(filament::math::quatf),
		BufferDescriptorCleanup<filament::math::quatf>,
		quats.release() );
	_filaGeom->_vb->setBufferAt( _engine, tangentBufferIndex,
		std::move(quatDesc) );
	
	// Maybe color data
	if (vertColors.get() != nullptr)
	{
		VertexBuffer::BufferDescriptor colorDesc( &(*vertColors)[0],
			inTMData->numPoints * sizeof(filament::math::float4),
			BufferDescriptorCleanup<TQ3ColorRGBA>,
			vertColors.release() );
		_filaGeom->_vb->setBufferAt( _engine, colorBufferIndex,
			std::move(colorDesc) );
	}
	
	// Bounds
	_filaGeom->_localBounds = BoundsToBox( inTMData->bBox );
	
	_filaGeom->_primitiveType = filament::backend::PrimitiveType::TRIANGLES;
	_filaGeom->_hasVertexColors = (vertColors.get() != nullptr);
}

void		GeomCacheImp::GetGeomTriMesh()
{
	CQ3ObjectRef nakedGeom( Q3TriMesh_GetNakedGeometry( _quesaGeom ) );
	TQ3Uns32 curEditIndex = Q3Shared_GetEditIndex( nakedGeom.get() );
	auto foundIt = _cache.find( nakedGeom.get() );
	if (foundIt != _cache.end())
	{
		_filaGeom = foundIt->second;
		if ( (not _filaGeom->_srcGeom.isvalid()) or
			(_filaGeom->_editIndex != curEditIndex) ) // stale cached item?
		{
			_filaGeom.reset();
			_cache.erase( foundIt );
		}
	}
	
	if (_filaGeom.get() == nullptr)
	{
		TQ3TriMeshData* tmData = nullptr;
		Q3TriMesh_LockData( _quesaGeom, kQ3True, &tmData );
		if (tmData != nullptr)
		{
			_filaGeom.reset( new FilaGeom );
			_filaGeom->_srcGeom.Assign( nakedGeom.get() );
			_filaGeom->_editIndex = curEditIndex;
			LoadTriMesh( tmData );
			
			Q3TriMesh_UnlockData( _quesaGeom );
			
			_cache[ nakedGeom.get() ] = _filaGeom;
		}
	}
}

bool	GeomCacheImp::LoadPolyLine()
{
	bool didLoad = false;
	TQ3PolyLineData	polyData;
	if (kQ3Success == Q3PolyLine_GetData( _quesaGeom, &polyData ))
	{
		assert( polyData.numVertices >= 2 );
		
		// Set up the index buffer
		IndexBuffer* theIndexBuf = IndexBuffer::Builder()
			.indexCount( 2 * (polyData.numVertices - 1) )
			.bufferType( IndexBuffer::IndexType::UINT )
			.build( _engine );
		_filaGeom->_indexBufferLines.reset( theIndexBuf, &_engine );
		std::unique_ptr< IndexVec > indices( new IndexVec(
			theIndexBuf->getIndexCount() ) );
		TQ3Uns32 j = 0;
		for (TQ3Uns32 i = 0; i < polyData.numVertices - 1; ++i)
		{
			(*indices)[j++] = i;
			(*indices)[j++] = i + 1;
		}
		IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
			theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
			BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
		_filaGeom->_indexBufferLines->setBuffer( _engine, std::move(indexDesc) );
		
		// Gather vertex data
		std::unique_ptr< ColorVec > colors( new ColorVec( polyData.numVertices ) );
		std::unique_ptr< PositionVec > positions( new PositionVec(
			polyData.numVertices ) );
		TQ3ColorRGB transColor;
		bool haveColor = true;
		for (TQ3Uns32 i = 0; i < polyData.numVertices; ++i)
		{
			(*positions)[i] = polyData.vertices[i].point;
			TQ3AttributeSet atts = polyData.vertices[i].attributeSet;
			if ( (atts == nullptr) or (kQ3Failure == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeDiffuseColor, &(*colors)[i] )) )
			{
				haveColor = false;
			}
			if ( (atts != nullptr) and (kQ3Success == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeTransparencyColor, &transColor )) )
			{
				(*colors)[i].a = (transColor.r + transColor.g + transColor.b) / 3.0f;
			}
			else
			{
				(*colors)[i].a = 1.0f;	// assume opaque
			}
		}
		
		// Set up the vertex buffer
		VertexBuffer::Builder vbBuild;
		vbBuild.vertexCount( polyData.numVertices )
			.bufferCount( haveColor? 2 : 1 )
			.attribute( VertexAttribute::POSITION, 0,
				VertexBuffer::AttributeType::FLOAT3 );
		if (haveColor)
		{
			vbBuild.attribute( VertexAttribute::COLOR, 1,
				VertexBuffer::AttributeType::FLOAT4 );
		}
		_filaGeom->_vb.reset( vbBuild.build( _engine ), &_engine );
		TQ3BoundingBox bounds;
		Q3BoundingBox_SetFromPoints3D( &bounds, &(*positions)[0],
			polyData.numVertices, sizeof(TQ3Point3D) );
		_filaGeom->_localBounds = BoundsToBox( bounds );
		
		VertexBuffer::BufferDescriptor posDesc( &(*positions)[0],
			polyData.numVertices * sizeof(TQ3Point3D),
			BufferDescriptorCleanup< TQ3Point3D >, positions.release() );
		_filaGeom->_vb->setBufferAt( _engine, 0, std::move(posDesc) );
		
		if (haveColor)
		{
			VertexBuffer::BufferDescriptor colorDesc( &(*colors)[0],
				polyData.numVertices * sizeof(TQ3ColorRGBA),
				BufferDescriptorCleanup< TQ3Point3D >, colors.release() );
			_filaGeom->_vb->setBufferAt( _engine, 1, std::move(colorDesc) );
		}
		
		Q3PolyLine_EmptyData( &polyData );

		_filaGeom->_primitiveType = filament::backend::PrimitiveType::LINES;
		_filaGeom->_hasVertexColors = false;

		didLoad = true;
	}
	
	return didLoad;
}

bool	GeomCacheImp::LoadTriangle()
{
	bool didLoad = false;
	TQ3TriangleData data;
	if (kQ3Success == Q3Triangle_GetData( _quesaGeom, &data ))
	{
		// Gather vertex data
		std::unique_ptr< PositionVec > positions( new PositionVec( 3 ) );
		std::unique_ptr< ColorVec > colors( new ColorVec( 3 ) );
		std::unique_ptr< UVVec > uvs( new UVVec( 3 ) );
		std::unique_ptr< QuatVec > quats( new QuatVec( 3 ) );
		TQ3ColorRGB transColor;
		bool haveColor = true;
		bool haveUV = true;
		for (int i = 0; i < 3; ++i)
		{
			(*positions)[i] = data.vertices[i].point;
			TQ3AttributeSet atts = data.vertices[i].attributeSet;
			if (kQ3Failure == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeDiffuseColor, &(*colors)[i] ))
			{
				haveColor = false;
			}
			if (kQ3Success == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeTransparencyColor, &transColor ))
			{
				(*colors)[i].a = (transColor.r + transColor.g + transColor.b) / 3.0f;
			}
			else
			{
				(*colors)[i].a = 1.0f;	// assume opaque
			}
			if ( (kQ3Failure == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeSurfaceUV, &(*uvs)[i] )) and
				(kQ3Failure == Q3AttributeSet_Get( atts,
				kQ3AttributeTypeShadingUV, &(*uvs)[i] )) )
			{
				haveUV = false;
			}
		}
		
		// If we don't have UVs, make up some, for later simplity
		if (not haveUV)
		{
			(*uvs)[0] = TQ3Param2D{ 0.0f, 0.0f };
			(*uvs)[1] = TQ3Param2D{ 1.0f, 0.0f };
			(*uvs)[2] = TQ3Param2D{ 0.5f, 1.0f };
		}
		
		// Generate normals
		TQ3Vector3D faceNormal;
		Q3Point3D_CrossProductTri( &data.vertices[0].point,
			&data.vertices[1].point, &data.vertices[2].point, &faceNormal );
		std::vector<TQ3Vector3D> vertNorms( 3, faceNormal );
		
		// Generate surface orientation quaternions
		math::uint3	triangle( 0, 1, 2 );
		geometry::SurfaceOrientation::Builder builder;
		builder.vertexCount( 3 )
			.normals( reinterpret_cast<const math::float3*>( &vertNorms[0] ) )
			.triangleCount( 1 )
			.triangles( &triangle )
			.positions( reinterpret_cast<const math::float3*>( &(*positions)[0] ) )
			.uvs( reinterpret_cast<const math::float2*>( &(*uvs)[0] ) );
		geometry::SurfaceOrientation* orient = builder.build();
		assert( orient != nullptr );
		orient->getQuats( &(*quats)[0], 3 );
		
		// Set up the index buffer
		IndexBuffer* theIndexBuf = IndexBuffer::Builder()
			.indexCount( 3 )
			.bufferType( IndexBuffer::IndexType::UINT )
			.build( _engine );
		_filaGeom->_indexBufferTriangles.reset( theIndexBuf, &_engine );
		std::unique_ptr< IndexVec > indices( new IndexVec( 3 ) );
		(*indices)[0] = 0;
		(*indices)[1] = 1;
		(*indices)[2] = 2;
		IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
			theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
			BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
		_filaGeom->_indexBufferTriangles->setBuffer( _engine, std::move(indexDesc) );
		
		// Set up the vertex buffer
		// We always have at least POSITION, UV0, and TANGENTS, maybe COLOR.
		uint8_t numVertexBuffers = 3;
		uint8_t postionBufferIndex = 0;	// VertexAttribute::POSITION
		uint8_t uvBufferIndex = 1;		// VertexAttribute::UV0
		uint8_t tangentBufferIndex = 2;	// VertexAttribute::TANGENTS
		uint8_t	colorBufferIndex = 3;	// VertexAttribute::COLOR, if haveColor
		if (haveColor)
		{
			++numVertexBuffers;
		}
		VertexBuffer::Builder vbBuild;
		vbBuild.bufferCount( numVertexBuffers )
			.vertexCount( 3 )
			.attribute( VertexAttribute::POSITION, postionBufferIndex,
				VertexBuffer::AttributeType::FLOAT3 )
			.attribute( VertexAttribute::UV0, uvBufferIndex,
				VertexBuffer::AttributeType::FLOAT2 )
			.attribute( VertexAttribute::TANGENTS, tangentBufferIndex,
				VertexBuffer::AttributeType::FLOAT4 );
		if (haveColor)
		{
			vbBuild.attribute( VertexAttribute::COLOR, colorBufferIndex,
				VertexBuffer::AttributeType::FLOAT4 );
		}
		_filaGeom->_vb.reset( vbBuild.build( _engine ), &_engine );

		// Bounds
		TQ3BoundingBox bounds;
		Q3BoundingBox_SetFromPoints3D( &bounds, &(*positions)[0], 3, sizeof(TQ3Point3D) );
		_filaGeom->_localBounds = BoundsToBox( bounds );
		
		_filaGeom->_primitiveType = filament::backend::PrimitiveType::TRIANGLES;
		_filaGeom->_hasVertexColors = haveColor;

		// supply positions
		VertexBuffer::BufferDescriptor posDesc( &(*positions)[0],
			3 * sizeof(TQ3Point3D),
			BufferDescriptorCleanup< TQ3Point3D >, positions.release() );
		_filaGeom->_vb->setBufferAt( _engine, 0, std::move(posDesc) );
		
		// Supply UVs
		VertexBuffer::BufferDescriptor uvDesc( &(*uvs)[0],
			3 * sizeof(TQ3Param2D),
			BufferDescriptorCleanup<TQ3Param2D>, uvs.release() );
		_filaGeom->_vb->setBufferAt( _engine, uvBufferIndex, std::move(uvDesc) );
		
		// Supply tangents
		VertexBuffer::BufferDescriptor tangentDesc( &(*quats)[0],
			3 * sizeof(math::quatf),
			BufferDescriptorCleanup<math::quatf>, quats.release() );
		_filaGeom->_vb->setBufferAt( _engine, tangentBufferIndex, std::move(tangentDesc) );
		
		// Supply color
		if (haveColor)
		{
			VertexBuffer::BufferDescriptor colorDesc( &(*colors)[0],
				3 * sizeof(TQ3ColorRGBA),
				BufferDescriptorCleanup<TQ3Param2D>, colors.release() );
			_filaGeom->_vb->setBufferAt( _engine, colorBufferIndex, std::move(colorDesc) );
		}
		
		Q3Triangle_EmptyData( &data );
		didLoad = true;
	}
	
	return didLoad;
}

void	GeomCacheImp::GetTriangleEdgeIndices()
{
	IndexBuffer* theIndexBuf = IndexBuffer::Builder()
		.indexCount( 6 )
		.bufferType( IndexBuffer::IndexType::UINT )
		.build( _engine );
	_filaGeom->_indexBufferLines.reset( theIndexBuf, &_engine );
	std::unique_ptr< IndexVec > indices( new IndexVec( 6 ) );
	indices->assign( {0, 1, 1, 2, 2, 0} );
	IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
		theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
		BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
	_filaGeom->_indexBufferLines->setBuffer( _engine, std::move(indexDesc) );
}

void	GeomCacheImp::GetTrMeshEdgeIndices()
{
	TQ3TriMeshData* tmData = nullptr;
	Q3TriMesh_LockData( _quesaGeom, kQ3True, &tmData );
	if (tmData != nullptr)
	{
		if (tmData->numEdges == 0)
		{
			GetTriMeshImplicitEdgeIndices( *tmData );
		}
		else
		{
			GetTriMeshExplicitEdgeIndices( *tmData );
		}
	
		Q3TriMesh_UnlockData( _quesaGeom );
	}
}

static IndexPair MakeOrderedEdge( TQ3Uns32 ind1, TQ3Uns32 ind2 )
{
	IndexPair aPair;
	if (ind1 <= ind2)
	{
		aPair.first = ind1;
		aPair.second = ind2;
	}
	else
	{
		aPair.first = ind2;
		aPair.second = ind1;
	}
	return aPair;
}

void	GeomCacheImp::GetTriMeshImplicitEdgeIndices( const TQ3TriMeshData& inTMData )
{
	// Build a set of edges to avoid duplications.
	EdgeSet edges;
	for (TQ3Uns32 faceIndex = 0; faceIndex < inTMData.numTriangles; ++faceIndex)
	{
		const TQ3TriMeshTriangleData& theFace( inTMData.triangles[ faceIndex ] );
		edges.insert( MakeOrderedEdge( theFace.pointIndices[0], theFace.pointIndices[1] ) );
		edges.insert( MakeOrderedEdge( theFace.pointIndices[0], theFace.pointIndices[2] ) );
		edges.insert( MakeOrderedEdge( theFace.pointIndices[1], theFace.pointIndices[2] ) );
	}

	IndexBuffer* theIndexBuf = IndexBuffer::Builder()
		.indexCount( static_cast<uint32_t>( 2 * edges.size() ) )
		.bufferType( IndexBuffer::IndexType::UINT )
		.build( _engine );
	_filaGeom->_indexBufferLines.reset( theIndexBuf, &_engine );
	std::unique_ptr< IndexVec > indices( new IndexVec );
	indices->reserve( theIndexBuf->getIndexCount() );
	
	for (const IndexPair& oneEdge : edges)
	{
		indices->push_back( oneEdge.first );
		indices->push_back( oneEdge.second );
	}

	IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
		theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
		BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
	_filaGeom->_indexBufferLines->setBuffer( _engine, std::move(indexDesc) );
}

void	GeomCacheImp::GetTriMeshExplicitEdgeIndices( const TQ3TriMeshData& inTMData )
{
	IndexBuffer* theIndexBuf = IndexBuffer::Builder()
		.indexCount( 2 * inTMData.numEdges )
		.bufferType( IndexBuffer::IndexType::UINT )
		.build( _engine );
	_filaGeom->_indexBufferLines.reset( theIndexBuf, &_engine );
	std::unique_ptr< IndexVec > indices( new IndexVec( theIndexBuf->getIndexCount() ) );
	for (TQ3Uns32 i = 0; i < inTMData.numEdges; ++i)
	{
		const TQ3TriMeshEdgeData& theEdge( inTMData.edges[i] );
		(*indices)[ 2 * i ] = theEdge.pointIndices[0];
		(*indices)[ 2 * i + 1 ] = theEdge.pointIndices[1];
	}
	IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
		theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
		BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
	_filaGeom->_indexBufferLines->setBuffer( _engine, std::move(indexDesc) );
}


bool	GeomCacheImp::LoadLine()
{
	bool didLoad = false;
	TQ3Point3D points[2];
	if ( (kQ3Success == Q3Line_GetVertexPosition( _quesaGeom, 0, &points[0]) ) and
		(kQ3Success == Q3Line_GetVertexPosition( _quesaGeom, 1, &points[1]) ) )
	{
		// Set up the index buffer
		IndexBuffer* theIndexBuf = IndexBuffer::Builder()
			.indexCount( 2 )
			.bufferType( IndexBuffer::IndexType::UINT )
			.build( _engine );
		_filaGeom->_indexBufferLines.reset( theIndexBuf, &_engine );
		std::unique_ptr< IndexVec > indices( new IndexVec( 2 ) );
		(*indices)[0] = 0;
		(*indices)[1] = 1;
		IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
			theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
			BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
		_filaGeom->_indexBufferLines->setBuffer( _engine, std::move(indexDesc) );
		
		// Set up the vertex buffer
		VertexBuffer::Builder vbBuild;
		vbBuild.vertexCount( 2 )
			.bufferCount( 1 )
			.attribute( VertexAttribute::POSITION, 0,
				VertexBuffer::AttributeType::FLOAT3 );
		_filaGeom->_vb.reset( vbBuild.build( _engine ), &_engine );
		std::unique_ptr< PositionVec > positions( new PositionVec(
			&points[0], &points[2] ) );
		VertexBuffer::BufferDescriptor posDesc( &(*positions)[0],
			2 * sizeof(TQ3Point3D),
			BufferDescriptorCleanup< TQ3Point3D >, positions.release() );
		_filaGeom->_vb->setBufferAt( _engine, 0, std::move(posDesc) );
		
		// Bounds
		TQ3BoundingBox bounds;
		Q3BoundingBox_SetFromPoints3D( &bounds, points, 2, sizeof(TQ3Point3D) );
		_filaGeom->_localBounds = BoundsToBox( bounds );
		
		_filaGeom->_primitiveType = filament::backend::PrimitiveType::LINES;
		_filaGeom->_hasVertexColors = false;
		
		didLoad = true;
	}
	return didLoad;
}

bool	GeomCacheImp::LoadPoint()
{
	bool didLoad = false;
	TQ3Point3D thePosition;
	if (kQ3Success == Q3Point_GetPosition( _quesaGeom, &thePosition ))
	{
		// Set up the index buffer
		IndexBuffer* theIndexBuf = IndexBuffer::Builder()
			.indexCount( 1 )
			.bufferType( IndexBuffer::IndexType::UINT )
			.build( _engine );
		_filaGeom->_indexBufferPoints.reset( theIndexBuf, &_engine );
		std::unique_ptr< IndexVec > indices( new IndexVec( 1 ) );
		(*indices)[0] = 0;
		IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
			theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
			BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
		_filaGeom->_indexBufferPoints->setBuffer( _engine, std::move(indexDesc) );
		
		// Set up the vertex buffer
		VertexBuffer::Builder vbBuild;
		vbBuild.vertexCount( 1 )
			.bufferCount( 1 )
			.attribute( VertexAttribute::POSITION, 0,
				VertexBuffer::AttributeType::FLOAT3 );
		_filaGeom->_vb.reset( vbBuild.build( _engine ), &_engine );
		std::unique_ptr< PositionVec > positions( new PositionVec( 1 ) );
		(*positions)[0] = thePosition;
		VertexBuffer::BufferDescriptor posDesc( &(*positions)[0],
			sizeof(TQ3Point3D),
			BufferDescriptorCleanup< TQ3Point3D >, positions.release() );
		_filaGeom->_vb->setBufferAt( _engine, 0, std::move(posDesc) );
		
		// Bounds
		TQ3BoundingBox bounds;
		bounds.min = bounds.max = thePosition;
		bounds.isEmpty = kQ3False;
		_filaGeom->_localBounds = BoundsToBox( bounds );
		
		_filaGeom->_primitiveType = filament::backend::PrimitiveType::POINTS;
		_filaGeom->_hasVertexColors = false;
		
		didLoad = true;
	}
	
	return didLoad;
}

void		GeomCacheImp::GetGeomNonTriMesh()
{
	// Look for it in the cache and make sure the cached version is not stale
	TQ3Uns32 curEditIndex = Q3Shared_GetEditIndex( _quesaGeom );
	auto foundIt = _cache.find( _quesaGeom );
	if (foundIt != _cache.end())
	{
		_filaGeom = foundIt->second;
		if ( (not _filaGeom->_srcGeom.isvalid()) or
			(_filaGeom->_editIndex != curEditIndex) ) // stale cached item?
		{
			_filaGeom.reset();
			_cache.erase( foundIt );
		}
	}
	
	// If no luck in the cache, add it to the cache.
	if (_filaGeom.get() == nullptr)
	{
		_filaGeom.reset( new FilaGeom );
		_filaGeom->_srcGeom.Assign( _quesaGeom );
		_filaGeom->_editIndex = curEditIndex;
		
		if (Q3Object_IsType( _quesaGeom, kQ3GeometryTypePolyLine ))
		{
			if (not LoadPolyLine())
			{
				_filaGeom.reset();
			}
		}
		else if (Q3Object_IsType( _quesaGeom, kQ3GeometryTypeLine ))
		{
			if (not LoadLine())
			{
				_filaGeom.reset();
			}
		}
		else if (Q3Object_IsType( _quesaGeom, kQ3GeometryTypePoint ))
		{
			if (not LoadPoint())
			{
				_filaGeom.reset();
			}
		}
		else if (Q3Object_IsType( _quesaGeom, kQ3GeometryTypeTriangle ))
		{
			if (not LoadTriangle())
			{
				_filaGeom.reset();
			}
		}
		else
		{
			_filaGeom.reset();
		}
		
		if (_filaGeom.get() != nullptr)
		{
			_cache[ _quesaGeom ] = _filaGeom;
		}
	}
}

/*!
	@function		GetGeom
	
	@abstract		Create, or retrieve from the cache, Filament geometry data for a TriMesh.
	@param			inGeom		A TriMesh.
	@param			inFillStyle	The current fill style.
	@result			A wrapped FilaGeom, or wrapped nil if something went wrong.
*/
sharedFilaGeom		GeomCacheImp::GetGeom( TQ3GeometryObject inGeom,
											TQ3FillStyle inFillStyle )
{
	_quesaGeom = inGeom;
	_filaGeom.reset();
	
	if (Q3Object_IsType( inGeom, kQ3GeometryTypeTriMesh ))
	{
		GetGeomTriMesh();
	}
	else if (Q3Object_IsType( inGeom, kQ3GeometryTypePolyLine ) or
		Q3Object_IsType( inGeom, kQ3GeometryTypeLine ) or
		Q3Object_IsType( inGeom, kQ3GeometryTypeTriangle ) or
		Q3Object_IsType( inGeom, kQ3GeometryTypePoint ) )
	{
		GetGeomNonTriMesh();
	}
	else
	{
		TQ3ObjectType obType = Q3Object_GetLeafType( inGeom );
		LogToConsole( "GeomCache passed unexpected type '%c%c%c%c'",
			(char)(obType >> 24), (char)(obType >> 16),
			(char)(obType >> 8), (char)(obType >> 0) );
	}
	
	// If we are asked for edges of 2D geometry, we may need another step.
	if ( (_filaGeom.get() != nullptr) and
		(inFillStyle == kQ3FillStyleEdges) and
		(_filaGeom->_primitiveType == backend::PrimitiveType::TRIANGLES) and
		(_filaGeom->_indexBufferLines.get() == nullptr) )
	{
		if (Q3Object_IsType( inGeom, kQ3GeometryTypeTriMesh ))
		{
			GetTrMeshEdgeIndices();
		}
		else if (Q3Object_IsType( inGeom, kQ3GeometryTypeTriangle ))
		{
			GetTriangleEdgeIndices();
		}
	}
	
	sharedFilaGeom result = _filaGeom;
	_filaGeom.reset();
	
	return result;
}

/*!
	@function		Flush
	@abstract		Flush any cached FilaGeom records for which the original naked TriMesh
					no longer exists or has an outdated edit index.
*/
void				GeomCacheImp::Flush()
{
	auto endIt = _cache.end();
	auto i = _cache.begin();
	while (i != endIt)
	{
		auto next = i;
		++next;
		
		if ( (not i->second->_srcGeom.isvalid()) or
			(Q3Shared_GetEditIndex( i->first ) != i->second->_editIndex) )
		{
			_cache.erase( i );
			LogToConsole("Flushed a stale naked TriMesh");
		}
		
		i = next;
	}
}

///MARK: -

GeomCache::GeomCache( filament::Engine& inEngine )
	: _imp( new GeomCacheImp( inEngine ) )
{
}

GeomCache::~GeomCache()
{
}

/*!
	@function		GetGeom
	
	@abstract		Create, or retrieve from the cache, Filament geometry data for a TriMesh.
	@param			inGeom		A TriMesh.
	@result			A wrapped FilaGeom, or wrapped nil if something went wrong.
*/
sharedFilaGeom	GeomCache::GetGeom( TQ3GeometryObject inGeom, TQ3FillStyle inFillStyle )
{
	return _imp->GetGeom( inGeom, inFillStyle );
}

/*!
	@function		Flush
	@abstract		Flush any cached FilaGeom records for which the original naked TriMesh
					no longer exists or has an outdated edit index.
*/
void			GeomCache::Flush()
{
	_imp->Flush();
}

