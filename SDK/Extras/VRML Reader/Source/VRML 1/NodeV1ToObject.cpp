/*  NAME:
        NodeV1ToObject.cp

    DESCRIPTION:
        Main function to convert a VRML 1 node to a Quesa object.

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
#include "NodeV1ToObject.h"

#include "ConeV1ToObject.h"
#include "Coordinate3V1ToState.h"
#include "CubeV1ToObject.h"
#include "CVRMLReader.h"
#include "CylinderV1ToObject.h"
#include "GetCachedObject.h"
#include "IndexedFaceSetV1ToObject.h"
#include "IndexedLineSetV1ToObject.h"
#include "MaterialV1ToState.h"
#include "MaterialBindingV1ToState.h"
#include "MatrixTransformV1ToObject.h"
#include "NormalBindingV1ToState.h"
#include "NormalV1ToState.h"
#include "PointSetV1ToObject.h"
#include "PolyValue.h"
#include "RotationV1ToObject.h"
#include "ScaleV1ToObject.h"
#include "SeparatorV1ToObject.h"
#include "ShapeHintsV1ToState.h"
#include "SphereV1ToObject.h"
#include "Texture2V1ToObject.h"
#include "TextureCoordinate2V1ToState.h"
#include "TransformSeparatorV1ToObject.h"
#include "TransformV1ToObject.h"
#include "TranslationV1ToObject.h"

#include <ostream>

/*!
	@function	NodeV1ToObject
	
	@abstract	Attempt to convert a VRML 2 node to a Quesa object.
	
	@discussion	The reason that the node is not passed as a const reference is
				that whenever we successfully produce a Quesa object from a
				node, we cache that object in the node.  This is not only a
				speed optimization, it is also necessary to guarantee that we
				will have multiple Quesa references whenever there are multiple
				references to a VRML node.
	
	@param		ioNode		Node to convert.
	@param		ioReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	NodeV1ToObject( PolyValue& ioNode, CVRMLReader& ioReader )
{
	CQ3ObjectRef	theObject;
	
	if (ioNode.GetType() == PolyValue::kDataTypeDictionary)
	{
		theObject = GetCachedObject( ioNode );
		
		if (not theObject.isvalid())
		{
			const PolyValue&	nodeTypeValue( ioNode.GetDictionary()["[type]"] );
			const std::string&	nodeType( nodeTypeValue.GetString() );
			
			if (nodeType == "Sphere")
			{
				theObject = SphereV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Cone")
			{
				theObject = ConeV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Cube")
			{
				theObject = CubeV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Cylinder")
			{
				theObject = CylinderV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Texture2")
			{
				theObject = Texture2V1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "IndexedFaceSet")
			{
				theObject = IndexedFaceSetV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "IndexedLineSet")
			{
				theObject = IndexedLineSetV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "PointSet")
			{
				theObject = PointSetV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Material")
			{
				 MaterialV1ToState( ioNode, ioReader );
			}
			else if (nodeType == "MaterialBinding")
			{
				 MaterialBindingV1ToState( ioNode, ioReader );
			}
			else if (nodeType == "NormalBinding")
			{
				 NormalBindingV1ToState( ioNode, ioReader );
			}
			else if (nodeType == "ShapeHints")
			{
				 ShapeHintsV1ToState( ioNode, ioReader );
			}
			else if (nodeType == "Coordinate3")
			{
				 Coordinate3V1ToState( ioNode, ioReader );
			}
			else if (nodeType == "TextureCoordinate2")
			{
				 TextureCoordinate2V1ToState( ioNode, ioReader );
			}
			else if (nodeType == "Normal")
			{
				 NormalV1ToState( ioNode, ioReader );
			}
			else if (nodeType == "Separator")
			{
				 theObject = SeparatorV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "TransformSeparator")
			{
				 theObject = TransformSeparatorV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Translation")
			{
				 theObject = TranslationV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Transform")
			{
				 theObject = TransformV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Scale")
			{
				 theObject = ScaleV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "Rotation")
			{
				 theObject = RotationV1ToObject( ioNode, ioReader );
			}
			else if (nodeType == "MatrixTransform")
			{
				 theObject = MatrixTransformV1ToObject( ioNode, ioReader );
			}
			else if (ioReader.GetDebugStream() != NULL)
			{
				*ioReader.GetDebugStream() << "Unhandled node type '" <<
					nodeType << "'." << std::endl;
			}
		}
	}
	
	return theObject;
}
