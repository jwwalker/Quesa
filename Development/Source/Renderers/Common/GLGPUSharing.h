/*  NAME:
        GLGPUSharing.h

    DESCRIPTION:
        Header file for GLGPUSharing.cpp.

    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
#ifndef GLGPUSHARING_HDR
#define GLGPUSHARING_HDR


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------

#include "E3Prefix.h"
#include "GLPrefix.h"



//=============================================================================
//      Types
//-----------------------------------------------------------------------------

/*!
	@class			CQ3GPSharedCache
	@abstract		Abstract base class for an object containing data that is
					stored on the GPU and may be shared between several GL
					contexts.
*/
class CQ3GPSharedCache
{
public:
	virtual		~CQ3GPSharedCache();
};



//=============================================================================
//      Functions
//-----------------------------------------------------------------------------

/*!
	@function		GLGPUSharing_GetNextSharingBase
	@abstract		Get the next GL sharing group base.
	@discussion		When a GL context is being created, we usually want to
					make it share data with previously created GL contexts.
					The texture manager maintains a list of existing GL contexts
					grouped by sharing.  This function can be used to find one
					context in each sharing group.
	@param			glBase			NULL to begin iteration, or a value returned
									by a previous call to this function.
	@result			Next GL sharing context, or NULL when there are no more.
*/

TQ3GLContext		GLGPUSharing_GetNextSharingBase( TQ3GLContext glBase );


/*!
	@function		GLGPUSharing_AddContext
	@abstract		After successfully creating a new context, call this function
					to inform the GPU sharing manager.
	@param			newGLContext		The newly created context.
	@param			sharingBase			The existing context with which the new one
										shares data, or NULL.
*/
void				GLGPUSharing_AddContext( TQ3GLContext newGLContext,
											TQ3GLContext sharingBase );


/*!
	@function		GLGPUSharing_RemoveContext
	@abstract		Inform the GPU sharing manager that a GL context has been destroyed.
	@discussion		The given GL context will be forgotten from its sharing group.
					If there are no more contexts associated with the sharing group,
					then the sharing group will be disposed.
	@param			deadGLContext		A former GL context.
*/
void				GLGPUSharing_RemoveContext( TQ3GLContext deadGLContext );


/*!
	@function		GLGPUSharing_IsContextKnown
	@abstract		Test whether the sharing manager knows about an OpenGL context.
	@param			inGLContext			A GL context.
	@result			True if we find a sharing group for the context.
*/
bool				GLGPUSharing_IsContextKnown( TQ3GLContext inGLContext );

/*!
	@function		GLGPUSharing_GetCache
	@abstract		Locate a cache within a sharing group.
	@param			glContext			A GL context.
	@param			inCacheType			key code for type of cache.
	@result			A cache pointer, or NULL.
*/
CQ3GPSharedCache*	GLGPUSharing_GetCache( TQ3GLContext glContext,
											TQ3Uns32 inCacheType );

/*!
	@function		GLGPUSharing_AddCache
	@abstract		Add a cache to a sharing group.
	@discussion		This should only be called if GLGPUSharing_GetCache has
					returned NULL.  After this call, the sharing group takes
					ownership of the cache, so do not delete the cache. it will
					be deleted when the sharing group is deleted.
	@param			glContext			A GL context.
	@param			inCacheType			key code for type of cache.
	@param			inCache				A cache.
*/
void				GLGPUSharing_AddCache( TQ3GLContext glContext,
											TQ3Uns32 inCacheType,
											CQ3GPSharedCache* inCache );


#if Q3_DEBUG
/*!
	@function		GLGPUSharing_IsCacheValid
	@abstract		Test whether a cache exists, for debugging.
	@param			inCacheType			key code for type of cache.
	@param			inCache				A cache.
	@result			True if the cache is found.
*/
bool				GLGPUSharing_IsCacheValid(
										CQ3GPSharedCache* inCache,
										TQ3Uns32 inCacheType );
#endif

#endif	// GLGPUSHARING_HDR
