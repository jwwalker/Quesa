/*  NAME:
        SetGroupName.cp

    DESCRIPTION:
        Utility for Group and Transform objects.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "GetObjectName.h"
#include "IsKeyPresent.h"
#include "SetGroupName.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaCustomElements.h>
	#include <Quesa/QuesaGroup.h>
#else
	#include <CQ3ObjectRef.h>
	#include <QuesaCustomElements.h>
	#include <QuesaGroup.h>
#endif


/*!
	@function	SetGroupName
	
	@abstract	Having built a group from its children, set its name.
				If there is a singleton child, and both the child and the
				group have names, and those names are different, we want
				to be careful to preserve both names.
	
	@param		ioGroup		Result of ChildrenV2ToObject, probably a group.
	@param		ioNodeDict	Dictionary of the group node.
*/
void	SetGroupName( CQ3ObjectRef& ioGroup, PolyValue::Dictionary& ioNodeDict )
{
	// If this node was named with DEF, set that as the name of the
	// Quesa object.
	if (IsKeyPresent( ioNodeDict, "[name]" ))
	{
		PolyValue&	nameValue( ioNodeDict["[name]"] );
		const std::string&	theName( nameValue.GetString() );
	
		// Special case: if ioGroup already has a name (by virtue of
		// being a named only child), and the group node also has a name,
		// and the names are different, I want to preserve both names.
		std::string	childName( GetObjectName( ioGroup ) );
		
		if ( (not childName.empty()) and (not theName.empty()) and
			(childName != theName) )
		{
			CQ3ObjectRef	newGroup( Q3DisplayGroup_New() );
			ThrowIfNullQuesaOb_( newGroup );
			Q3Group_AddObject( newGroup.get(), ioGroup.get() );
			ioGroup = newGroup;
		}
		
		CENameElement_SetData( ioGroup.get(), theName.c_str() );
	}
}
