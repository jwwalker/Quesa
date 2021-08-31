//
//  CQ3AttributeSet_Inherit.h
//  FilamentViewer
//
//  Created by James Walker on 2/27/21.
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

#ifndef CQ3AttributeSet_Inherit_hpp
#define CQ3AttributeSet_Inherit_hpp

#import <Quesa/CQ3ObjectRef.h>

/*!
	@function	CQ3AttributeSet_Inherit
	
	@abstract	An easier to use version of Q3AttributeSet_Inherit.
	
	@discussion	If both attribute sets are non-NULL, this does the same thing as Q3AttributeSet_Inherit.
				If just one is non-NULL, it returns a new reference to that non-NULL one.  And if both
				are NULL, this returns a new empty attribute set.
	
	@param		inParent	An attribute set.
	@param		inChild		An attribute set that may override some attributes in the parent.
	@result		The merged attribute set.
*/
CQ3ObjectRef	CQ3AttributeSet_Inherit( TQ3AttributeSet _Nullable inParent,
										TQ3AttributeSet _Nullable inChild );

#endif /* CQ3AttributeSet_Inherit_hpp */
