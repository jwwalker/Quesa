//
//  FilaGeom.h
//  FilamentViewer
//
//  Created by James Walker on 3/24/21.
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

#ifndef FilaGeom_h
#define FilaGeom_h

#import "EngineSharedPtr.hpp"
#import <Quesa/CQ3WeakObjectRef.h>
#import <filament/Box.h>

enum TQ3FillStyle : unsigned int;

namespace filament
{
	namespace backend
	{
		enum class PrimitiveType : uint8_t;
	}
}


/*!
	@struct		FilaGeom
	
	@abstract	This structure wraps Filament geometry (but not material) information corresponding
				to a Quesa geometry.  It holds a weak reference to the original Quesa geometry, and
				its edit index at the time the FilaGeom was constructed, in order that cached
				FilaGeoms can be flushed when out of date.
*/
struct FilaGeom
{
							FilaGeom();
							
							FilaGeom( const FilaGeom& inOther ) = delete;
							FilaGeom( FilaGeom&& ioOther ) = delete;
							
							~FilaGeom();

	FilaGeom&				operator=( const FilaGeom& inOther ) = delete;
	FilaGeom&				operator=( FilaGeom&& inOther ) = delete;
	
	/*!
		@function			EffectivePrimitiveType
		@abstract			Get the primitive type that we will be rendering with the given
							fill style.  It may be a lower dimension than the source geometry.
		@param				inFillStyle		The current fill style.
		@result				The effective primitive type.
	*/
	filament::backend::PrimitiveType
							EffectivePrimitiveType( TQ3FillStyle inFillStyle ) const;

	/*!
		@function			IndexBuffer
		@abstract			Return an index buffer appropriate for the desired fill style.
		@param				inFillStyle		The current fill style.
		@result				An index buffer or NULL.
	*/
	sharedIndexBuffer		IndexBuffer( TQ3FillStyle inFillStyle );
	
	sharedVertexBuffer					_vb;
	sharedIndexBuffer					_indexBufferTriangles;
	sharedIndexBuffer					_indexBufferLines;
	sharedIndexBuffer					_indexBufferPoints;
	CQ3WeakObjectRef					_srcGeom;
	TQ3Uns32							_editIndex;
	filament::Box						_localBounds;
	filament::backend::PrimitiveType	_primitiveType;
	bool								_hasVertexColors;

private:
	void					CreatePointIndexBuffer();
};

typedef std::shared_ptr< FilaGeom >	sharedFilaGeom;


#endif /* FilaGeom_h */
