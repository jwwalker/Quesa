/*  NAME:
        ShapeV2ToObject.cp

    DESCRIPTION:
        VRML 2 node handler.

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
#include "ShapeV2ToObject.h"

#include "AppearanceV2ToObject.h"
#include "BoxV2ToObject.h"
#include "CVRMLReader.h"
#include "ConeV2ToObject.h"
#include "CylinderV2ToObject.h"
#include "GetCachedObject.h"
#include "IndexedFaceSetV2ToObject.h"
#include "IndexedLineSetV2ToObject.h"
#include "IsKeyPresent.h"
#include "PointSetV2ToObject.h"
#include "PolyValue.h"
#include "PrependObjectToGroup.h"
#include "SphereV2ToObject.h"

#if __MACH__
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaSet.h>
	#include <Quesa/QuesaCustomElements.h>
#else
	#include <QuesaGeometry.h>
	#include <QuesaGroup.h>
	#include <QuesaSet.h>
	#include <QuesaCustomElements.h>
#endif

#include <ostream>

static CQ3ObjectRef	MakeDefaultAttSet()
{
	CQ3ObjectRef	theAtts( Q3AttributeSet_New() );
	if (theAtts.isvalid())
	{
		TQ3ColorRGB		defaultColor = { 1.0f, 1.0f, 1.0f };
		Q3AttributeSet_Add( theAtts.get(), kQ3AttributeTypeDiffuseColor, &defaultColor );
	}
	return theAtts;
}

/*!
	@function	ShapeV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Shape node to a Quesa object.
	
	@param		ioNode			Node to convert.
	@param		inReader		The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	ShapeV2ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject;
	
	PolyValue&	theGeom( ioNode.GetDictionary()["geometry"] );
	
	if (theGeom.GetType() == PolyValue::kDataTypeDictionary)
	{
		CQ3ObjectRef	geomObject( GetCachedObject( theGeom ) );
		
		if (not geomObject.isvalid())
		{
			PolyValue::Dictionary	geomDict( theGeom.GetDictionary() );
			const PolyValue&	nodeTypeValue( geomDict["[type]"] );
			const std::string&	geomType( nodeTypeValue.GetString() );
			
			if (geomType == "Cylinder")
			{
				geomObject = CylinderV2ToObject( theGeom );
			}
			else if (geomType == "Cone")
			{
				geomObject = ConeV2ToObject( theGeom );
			}
			else if (geomType == "Sphere")
			{
				geomObject = SphereV2ToObject( theGeom );
			}
			else if (geomType == "Box")
			{
				geomObject = BoxV2ToObject( theGeom );
			}
			else if (geomType == "PointSet")
			{
				geomObject = PointSetV2ToObject( theGeom );
			}
			else if (geomType == "IndexedFaceSet")
			{
				geomObject = IndexedFaceSetV2ToObject( theGeom, inReader );
			}
			else if (geomType == "IndexedLineSet")
			{
				geomObject = IndexedLineSetV2ToObject( theGeom );
			}
			else if (inReader.GetDebugStream() != NULL)
			{
				*inReader.GetDebugStream() << "No handler for geometry node '" <<
					geomType << "'." << std::endl;
			}
			
			if (geomObject.isvalid())
			{
				// If this geometry was named with DEF, set that as the name of the
				// Quesa object.
				if (IsKeyPresent( geomDict, "[name]" ))
				{
					PolyValue&	nameValue( geomDict["[name]"] );
					const std::string&	theName( nameValue.GetString() );
					::CENameElement_SetData( geomObject.get(), theName.c_str() );
				}
			}
		}
		
		if (geomObject.isvalid())
		{
			CQ3ObjectRef	appearanceObject;
			PolyValue&	theAppearanceNode( ioNode.GetDictionary()["appearance"] );
			
			if (theAppearanceNode.GetType() == PolyValue::kDataTypeDictionary)
			{
				appearanceObject = GetCachedObject( theAppearanceNode );
				
				if (not appearanceObject.isvalid())
				{
					appearanceObject = MakeDefaultAttSet();
					AppearanceV2ToObject( theAppearanceNode, inReader, appearanceObject );
				}
			}
			else	// no Appearance node
			{
				appearanceObject = MakeDefaultAttSet();
			}
			
			theObject = CQ3ObjectRef( Q3Object_Duplicate( geomObject.get() ) );
			
			if (theObject.isvalid() and appearanceObject.isvalid())
			{
				if (Q3Object_IsType( theObject.get(), kQ3ShapeTypeGeometry ))
				{
					Q3Geometry_SetAttributeSet( theObject.get(), appearanceObject.get() );
				}
				else if (Q3Object_IsType( theObject.get(), kQ3ShapeTypeGroup ))
				{
					PrependObjectToGroup( appearanceObject, theObject );
				}
			}
		}
	}
	
	if (theObject.isvalid())
	{
		SetCachedObject( ioNode, theObject );
	}
	
	return theObject;
}
