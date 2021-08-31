//
//  FilaGeom.cpp
//  FilamentViewer
//
//  Created by James Walker on 6/4/21.
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

#include "FilaGeom.h"

#include "LogToConsole.h"

#include <Quesa/QuesaStyle.h>

#import <backend/DriverEnums.h>
#import <filament/IndexBuffer.h>
#import <filament/VertexBuffer.h>

#import <vector>
#import <memory>
#import <iostream>

using namespace filament;

namespace
{
	typedef std::vector< TQ3Uns32 >					IndexVec;
}

template <typename T>
inline void BufferDescriptorCleanup( void* buffer, size_t size, void* userData )
{
	std::vector< T >* bufVec = reinterpret_cast<std::vector< T >*>( userData );
	delete bufVec;
	LogToConsole("Released buffer storage of size %zu", size );
}


backend::PrimitiveType
FilaGeom::EffectivePrimitiveType( TQ3FillStyle inFillStyle ) const
{
	backend::PrimitiveType result = _primitiveType;
	
	if (inFillStyle == kQ3FillStyleEdges)
	{
		if (result == backend::PrimitiveType::TRIANGLES)
		{
			result = backend::PrimitiveType::LINES;
		}
	}
	else if (inFillStyle == kQ3FillStylePoints)
	{
		result = backend::PrimitiveType::POINTS;
	}
	
	return result;
}

static int sFilaGeomCount = 0;

FilaGeom::FilaGeom()
{
	++sFilaGeomCount;
	std::cout << "FilaGeom " << this << " created, count " << sFilaGeomCount << "\n";
}

FilaGeom::~FilaGeom()
{
	--sFilaGeomCount;
	std::cout << "FilaGeom " << this << " destroyed, count " << sFilaGeomCount << "\n";
}

void	FilaGeom::CreatePointIndexBuffer()
{
	const size_t vertCount = _vb->getVertexCount();
	std::unique_ptr< IndexVec > indices( new IndexVec( vertCount ) );
	for (size_t i = 0; i < vertCount; ++i)
	{
		(*indices)[i] = static_cast<TQ3Uns32>( i );
	}
	Engine* engine = _vb.engine();
	filament::IndexBuffer* theIndexBuf = IndexBuffer::Builder()
			.indexCount( static_cast<uint32_t>(vertCount) )
			.bufferType( IndexBuffer::IndexType::UINT )
			.build( *engine );
	_indexBufferPoints.reset( theIndexBuf, engine );
	IndexBuffer::BufferDescriptor indexDesc( &(*indices)[0],
		theIndexBuf->getIndexCount() * sizeof(TQ3Uns32),
		BufferDescriptorCleanup<TQ3Uns32>, indices.release() );
	_indexBufferPoints->setBuffer( *engine, std::move(indexDesc) );
}

sharedIndexBuffer	FilaGeom::IndexBuffer( TQ3FillStyle inFillStyle )
{
	sharedIndexBuffer result;
	
	backend::PrimitiveType typeToRender = EffectivePrimitiveType( inFillStyle );
	
	switch (typeToRender)
	{
		default:
		case backend::PrimitiveType::TRIANGLES:
			result = _indexBufferTriangles;
			break;
		
		case backend::PrimitiveType::LINES:
			result = _indexBufferLines;
			break;
		
		case backend::PrimitiveType::POINTS:
			result = _indexBufferPoints;
			if (result.get() == nullptr)
			{
				CreatePointIndexBuffer();
				result = _indexBufferPoints;
			}
			break;
	}
	
	return result;
}
