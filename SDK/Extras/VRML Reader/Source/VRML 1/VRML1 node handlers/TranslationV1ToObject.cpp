#include "TranslationV1ToObject.h"
/*  NAME:
        TranslationV1ToObject.cp

    DESCRIPTION:
        VRML 1 node handler.

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

#include "IsKeyPresent.h"
#include "GetCachedObject.h"
#include "PolyValue.h"

#if __MACH__
	#include <Quesa/QuesaTransform.h>
#else
	#include <QuesaTransform.h>
#endif

/*!
	@function	TranslationV1ToObject
	
	@abstract	Attempt to convert a VRML 1 Translation node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	TranslationV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
#pragma unused( inReader )
	CQ3ObjectRef	theTransform;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	if (IsKeyPresent( theDict, "translation" ))
	{
		PolyValue&	transValue( theDict["translation"] );
		if (transValue.IsNumberVec())
		{
			PolyValue::FloatVec& transVec( transValue.GetFloatVec() );
			if (transVec.size() == 3)
			{
				TQ3Vector3D	translate = {
					transVec[0], transVec[1], transVec[2]
				};
				theTransform = CQ3ObjectRef( Q3TranslateTransform_New( &translate ) );
				
				if (theTransform.isvalid())
				{
					SetCachedObject( ioNode, theTransform );
				}
			}
		}
	}
	
	return theTransform;
}
