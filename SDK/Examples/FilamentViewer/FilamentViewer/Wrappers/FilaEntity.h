//
//  FilaEntity.h
//  FilamentViewer
//
//  Created by James Walker on 4/18/21.
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

#ifndef FilaEntity_hpp
#define FilaEntity_hpp

#include <utils/Entity.h>


/*!
	@class		FilaEntity
	
	@abstract	This class wraps a Filament Entity to ensure that it gets cleaned up.
*/
class FilaEntity
{
public:
					FilaEntity();
					FilaEntity( const FilaEntity& inOther ) = delete;
					~FilaEntity();
	
	FilaEntity&		operator=( const FilaEntity& inOther ) = delete;
	
					operator utils::Entity() const;
	
	/*!
		@function	destroy
		@abstract	Destroy the entity, if you need to make sure it happens before the FilaEntity
					destructor is called.
	*/
	void			destroy();

private:
	utils::Entity	_entity;
};

#endif /* FilaEntity_hpp */
