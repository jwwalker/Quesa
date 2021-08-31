//
//  NewDecomposingRenderer.h
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

#ifndef NewDecomposingRenderer_hpp
#define NewDecomposingRenderer_hpp

#import <Quesa/CQ3ObjectRef.h>

/*!
	@function	NewDecomposingRenderer
	
	@abstract	Create a new decomposing Quesa renderer.
	
	@discussion	The "decomposing" renderer does not actually render anything, and does not claim
				to be able to render any types of geometry.
				
				For example, suppose an Ellipsoid geometry is submitted for rendering to a View
				subclass.  If the submit-for-render method of the View subclass handles this kind of
				geometry on its own, then that's the end of the story.  On the other hand, suppose
				that the submit-for-render method of the View subclass passes the buck to the
				submit-for-render method of the View base class.  Since this renderer does not have
				a submit-geometry method for this kind of geometry, Quesa will decompose the
				Ellipsoid to a TriMesh and then submit that to the View.
	
	@result		A new decomposing renderer.
*/
CQ3ObjectRef 	NewDecomposingRenderer( void );

#endif /* NewDecomposingRenderer_hpp */
