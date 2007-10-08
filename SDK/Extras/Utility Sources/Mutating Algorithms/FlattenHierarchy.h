/*  NAME:
        MergeTriMeshes.h

    DESCRIPTION:
        Quesa utility header.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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
#ifndef QUESA_FLATTENHIERARCHY_HDR
#define QUESA_FLATTENHIERARCHY_HDR

#if !TARGET_RT_MAC_MACHO
	#include "Quesa.h"
#else
	#include <Quesa/Quesa.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*!
	@function	FlattenHierarchy
	
	@abstract	Create a group that contains the same geometries as the given
				group but with only two levels of groupings.
	
	@discussion	This function creates a display group containing ordered display
				groups, each of which contains geometries and other objects from
				the original group hierarchy.  Each geometry will be subject to
				the same transforms, attribute sets, styles, and shaders as in
				the original group.
				
				One can specify that subgroups of the original hierarchy will be
				skipped if they lack certain display group state flags.  For
				example, passing kQ3DisplayGroupStateMaskIsDrawn as the
				inRequiredStateMask parameter means that invisible groups will
				be skipped.
	
	@param		inGroup				The original group.
	@param		inRequiredStateMask	Combination of display group flags to
									require, or 0 to include everything.
	
	@result		A reference to a new group.
*/
TQ3DisplayGroupObject	FlattenHierarchy(
								TQ3DisplayGroupObject inGroup,
								TQ3Uns32 inRequiredStateMask );


#ifdef __cplusplus
}
#endif

#endif
