//
//  NewDecomposingRenderer.cpp
//  FilamentViewer
//
//  Created by James Walker on 2/25/21.
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

#import "NewDecomposingRenderer.h"

#import "LogToConsole.h"

#import <Quesa/QuesaExtension.h>
#import <Quesa/QuesaRenderer.h>

#import <assert.h>

static TQ3ObjectType				sMyRendererType = 0;
static TQ3XObjectClass				sMyRendererClass = nullptr;


namespace
{
class InitClass
{
public:
	InitClass();
	
};
}

static TQ3XFunctionPointer rendererGeomSubMetaHandler( TQ3XMethodType methodType )
{
	return nullptr;
}

static TQ3XFunctionPointer
	rendererMetaHandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer method = nullptr;
	
	switch (methodType)
	{
		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			method = (TQ3XFunctionPointer) rendererGeomSubMetaHandler;
			break;
	}
	
	return method;
}

InitClass::InitClass()
{
	assert( sMyRendererType == 0 );

	// Register my subclass
	sMyRendererClass = Q3XObjectHierarchy_RegisterClass( kQ3RendererTypeGeneric,
		&sMyRendererType, "DecomposingRenderer", rendererMetaHandler, nullptr, 0, 0 );

	assert( sMyRendererType != 0 );
}

///MARK:-

/*!
	@function	NewDecomposingRenderer
	
	@abstract	Create a new decomposing Quesa renderer.
	
	@discussion	The "decomposing" renderer does not actually render anything, but reports success on
				rendering TriMesh, Triangle, Line, and Point geometries, and nothing else.  This
				will cause most other geometries, e.g. Cone or NurbCurve, to be decomposed into
				simpler parts and resubmitted.  In contrast, the generic renderer claims to render
				everything, and thus will not cause anything to be decomposed.
	
	@result		A new decomposing renderer.
*/
CQ3ObjectRef 	NewDecomposingRenderer( void )
{
	// Doing the initialization in a static initializer guarantees that it
	// will happen just once, even if this function is called from multiple
	// threads.
	static InitClass init;
	assert( sMyRendererType != 0 );
	
	CQ3ObjectRef result( Q3Renderer_NewFromType( sMyRendererType ) );
	
	return result;
}
