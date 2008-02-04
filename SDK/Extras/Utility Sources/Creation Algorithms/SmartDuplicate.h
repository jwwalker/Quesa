/*  NAME:
        SmartDuplicate.h

    DESCRIPTION:
        Quesa utility header.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2008, Quesa Developers. All rights reserved.

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
#ifndef QUESA_SMARTDUPLICATE_HDR
#define QUESA_SMARTDUPLICATE_HDR

#if !TARGET_RT_MAC_MACHO
	#include "Quesa.h"
#else
	#include <Quesa/Quesa.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*!
	@typedef	ShouldDuplicateProc
	
	@abstract	Callback by which SmartDuplicate inquires whether it should
				really make a duplicate, or just share a reference.
				
	@param		inObject		A Quesa object.
	@param		inContext		A pointer value that was passed to SmartDuplicate.
	@result		kQ3True if the object should be duplicated, kQ3False if it
				should be referenced.
*/
typedef TQ3Boolean	(*ShouldDuplicateProc)( TQ3Object inObject, void* inContext );

/*!
	@function	SmartDuplicate
	
	@abstract	Duplicate an object in a way that preserves sharing of
				references, unlike Q3Object_Duplicate.
				
	@discussion	For example, if the input object is a group containing two
				geometries which refer to the same attribute set, SmartDuplicate
				will create a group containing two geometries that again refer
				to a common attribute set.
				
				In some situations, you might not want to make true duplicates
				certain parts.  For example, you might want a duplicate of a
				group hierarchy in which any texture shaders are simply
				references to the same texture shaders used in the original
				group hierarhcy.  To accomplish that goal, you would provide a
				callback that returns kQ3False for texture shaders.
	
	@param		inObject	A Quesa shared object.
	@param		inContext	A pointer that will be passed to the callback.
							May be NULL.
	@param		inCallback	A callback that will be called during the
							duplication process.  You may pass NULL as an
							equivalent to a callback that always returns kQ3True.
	
	@result		A new duplicate object.
*/
TQ3SharedObject	SmartDuplicate( TQ3SharedObject inObject, void* inContext,
								ShouldDuplicateProc inCallback );


#ifdef __cplusplus
}
#endif

#endif
