/*  NAME:
        ParseVRML1.cp

    DESCRIPTION:
        Parse VRML 1 grammar.

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
#include "ParseVRML1.h"

#include "ConvertNumericArray.h"
#include "UnBackslashEscape.h"
#include "VRML_1_constants.h"
#include "VRMLSkipParser.h"

#include <ostream>
#include <cctype>

using namespace boost::spirit;

	const char*	kEnumJustification_LEFT					= "LEFT";
	const char*	kEnumJustification_CENTER				= "CENTER";
	const char*	kEnumJustification_RIGHT				= "RIGHT";

	const char*	kEnumFamily_SERIF						= "SERIF";
	const char*	kEnumFamily_SANS						= "SANS";
	const char*	kEnumFamily_TYPEWRITER					= "TYPEWRITER";

	const char*	kEnumValue_DEFAULT						= "DEFAULT";
	const char*	kEnumValue_OVERALL						= "OVERALL";
	const char*	kEnumValue_PER_PART						= "PER_PART";
	const char*	kEnumValue_PER_PART_INDEXED				= "PER_PART_INDEXED";
	const char*	kEnumValue_PER_FACE						= "PER_FACE";
	const char*	kEnumValue_PER_FACE_INDEXED				= "PER_FACE_INDEXED";
	const char*	kEnumValue_PER_VERTEX					= "PER_VERTEX";
	const char*	kEnumValue_PER_VERTEX_INDEXED			= "PER_VERTEX_INDEXED";

	const char*	kEnumCulling_ON							= "ON";
	const char*	kEnumCulling_OFF						= "OFF";
	const char*	kEnumCulling_AUTO						= "AUTO";

	const char*	kEnumVertexOrdering_UNKNOWN_ORDERING	= "UNKNOWN_ORDERING";
	const char*	kEnumVertexOrdering_COUNTERCLOCKWISE	= "COUNTERCLOCKWISE";
	const char*	kEnumVertexOrdering_CLOCKWISE			= "CLOCKWISE";

	const char*	kEnumShapeType_UNKNOWN_SHAPE_TYPE		= "UNKNOWN_SHAPE_TYPE";
	const char*	kEnumShapeType_SOLID					= "SOLID";

	const char*	kEnumFaceType_UNKNOWN_FACE_TYPE			= "UNKNOWN_FACE_TYPE";
	const char*	kEnumFaceType_CONVEX					= "CONVEX";

	const char*	kEnumWrap_REPEAT						= "REPEAT";
	const char*	kEnumWrap_CLAMP							= "CLAMP";

	const char*	kEnumMap_NONE							= "NONE";
	const char*	kEnumMap_POINT							= "POINT";

	const char*	kMaskParts_SIDES						= "SIDES";
	const char*	kMaskParts_TOP							= "TOP";
	const char*	kMaskParts_BOTTOM						= "BOTTOM";
	const char*	kMaskParts_ALL							= "ALL";

	const char*	kMaskStyle_NONE							= "NONE";
	const char*	kMaskStyle_BOLD							= "BOLD";
	const char*	kMaskStyle_ITALIC						= "ITALIC";

namespace
{
	struct SParseState;
	
	#pragma mark struct VRMLParser
	struct VRMLParser : public grammar<VRMLParser>
	{
						VRMLParser( SParseState& inState ) : mState( inState ) {}
		
		template <typename ScannerT>
		struct definition
		{
			definition( const VRMLParser& self );
			
			const rule<ScannerT>& start() const
			{
				return startRule;
			}
			
			rule<ScannerT>	startRule;
			rule<ScannerT>	fieldID, nodeTypeID, nodeNameID;
			rule<ScannerT>	sfBoolValue, numArrayValue;
			rule<ScannerT>	sfNodeValue, node, nodeDeclaration;
			rule<ScannerT>	fieldValue;
			rule<ScannerT>	numArrayMember;
			rule<ScannerT>	arrayValue;
			rule<ScannerT>	nodeGut, nodeGuts;
			rule<ScannerT>	sfStringValue;
			rule<ScannerT>	enumOrMask;
		};
		
		SParseState&		mState;
	};
	
	#pragma mark struct SParseState
	struct SParseState
	{
								SParseState(
										PolyValue::PolyVec& ioTopNodes,
										std::ostream* ioStream );
		
		void					ConvertNumericArray( bool inConvertSingleToFloat );
		
		VRMLParser				mGrammar;
		symbols<int>			mConstants;
		symbols<int>			mBools;
		std::ostream*			mStream;
		PolyValue::PolyVec		mProgressStack;
		PolyValue::PolyVec&		mCompleteTopNodes;
		PolyValue::Dictionary	mNamedNodes;
	};

	#pragma mark === Action declarations ===
	#define	DECLARE_NORMAL_ACTION( Name )	\
			struct Name {					\
				Name( SParseState& ioState ) : mState( ioState ) {}	\
				Name( const Name& inOther ) : mState( inOther.mState ) {}	\
				void	operator()( const char* inStart, const char* inEnd ) const;	\
				SParseState&		mState;		\
			}
	
	#define	DECLARE_CHAR_ACTION( Name )	\
			struct Name {					\
				Name( SParseState& ioState ) : mState( ioState ) {}	\
				Name( const Name& inOther ) : mState( inOther.mState ) {}	\
				void	operator()( char inChar ) const;	\
				SParseState&		mState;		\
			}
	
	#define	DECLARE_FLOAT_ACTION( Name )	\
			struct Name {					\
				Name( SParseState& ioState ) : mState( ioState ) {}	\
				Name( const Name& inOther ) : mState( inOther.mState ) {}	\
				void	operator()( double inValue ) const;	\
				SParseState&		mState;		\
			}

	
	#define	DECLARE_INT_ACTION( Name )	\
			struct Name {					\
				Name( SParseState& ioState ) : mState( ioState ) {}	\
				Name( const Name& inOther ) : mState( inOther.mState ) {}	\
				void	operator()( int inValue ) const;	\
				SParseState&		mState;		\
			}
	
	DECLARE_NORMAL_ACTION( PushString );
	DECLARE_NORMAL_ACTION( PushQuotedString );
	DECLARE_NORMAL_ACTION( PushNULL );
	DECLARE_NORMAL_ACTION( FinishDEF );
	DECLARE_NORMAL_ACTION( FinishUSE );
	DECLARE_NORMAL_ACTION( FinishTopNode );
	DECLARE_NORMAL_ACTION( AppendNodeToArray );
	DECLARE_NORMAL_ACTION( FinishField );
	DECLARE_NORMAL_ACTION( FinishMask );
	DECLARE_NORMAL_ACTION( FinishUnbracketedArray );
	DECLARE_NORMAL_ACTION( FinishNodeChildren );
	DECLARE_NORMAL_ACTION( AppendQuotedStringToArray );
	
	DECLARE_CHAR_ACTION( StartNode );
	DECLARE_CHAR_ACTION( StartArray );
	DECLARE_CHAR_ACTION( FinishArray );
	DECLARE_CHAR_ACTION( FinishStringArray );
	
	DECLARE_FLOAT_ACTION( AppendFloatToPolyArray );
	
	DECLARE_INT_ACTION( PushBool );
	DECLARE_INT_ACTION( AppendIntToPolyArray );
	DECLARE_INT_ACTION( StartMask );
	DECLARE_INT_ACTION( MoreMask );
}

SParseState::SParseState( PolyValue::PolyVec& ioTopNodes, std::ostream* ioStream )
	: mGrammar( *this ),
	mStream( ioStream ),
	mCompleteTopNodes( ioTopNodes )
{
	mConstants.add
		( kEnumJustification_LEFT, eVRML1Justification_LEFT )
		( kEnumJustification_CENTER, eVRML1Justification_CENTER )
		( kEnumJustification_RIGHT, eVRML1Justification_RIGHT )
		( kEnumFamily_SERIF, eVRML1Family_SERIF )
		( kEnumFamily_SANS, eVRML1Family_SANS )
		( kEnumFamily_TYPEWRITER, eVRML1Family_TYPEWRITER )
		( kEnumValue_DEFAULT, eVRML1Value_DEFAULT )
		( kEnumValue_OVERALL, eVRML1Value_OVERALL )
		( kEnumValue_PER_PART, eVRML1Value_PER_PART )
		( kEnumValue_PER_PART_INDEXED, eVRML1Value_PER_PART_INDEXED )
		( kEnumValue_PER_FACE, eVRML1Value_PER_FACE )
		( kEnumValue_PER_FACE_INDEXED, eVRML1Value_PER_PART_INDEXED )
		( kEnumValue_PER_PART_INDEXED, eVRML1Value_PER_FACE_INDEXED )
		( kEnumValue_PER_VERTEX, eVRML1Value_PER_VERTEX )
		( kEnumValue_PER_VERTEX_INDEXED, eVRML1Value_PER_VERTEX_INDEXED )
		( kEnumCulling_ON, eVRML1Culling_ON )
		( kEnumCulling_OFF, eVRML1Culling_OFF )
		( kEnumCulling_AUTO, eVRML1Culling_AUTO )
		( kEnumVertexOrdering_UNKNOWN_ORDERING, eVRML1VertexOrdering_UNKNOWN_ORDERING )
		( kEnumVertexOrdering_COUNTERCLOCKWISE, eVRML1VertexOrdering_COUNTERCLOCKWISE )
		( kEnumVertexOrdering_CLOCKWISE, eVRML1VertexOrdering_CLOCKWISE )
		( kEnumShapeType_UNKNOWN_SHAPE_TYPE, eVRML1ShapeType_UNKNOWN_SHAPE_TYPE )
		( kEnumShapeType_SOLID, eVRML1ShapeType_SOLID )
		( kEnumFaceType_UNKNOWN_FACE_TYPE, eVRML1FaceType_UNKNOWN_FACE_TYPE )
		( kEnumFaceType_CONVEX, eVRML1FaceType_CONVEX )
		( kEnumWrap_REPEAT, eVRML1Wrap_REPEAT )
		( kEnumWrap_CLAMP, eVRML1Wrap_CLAMP )
		( kEnumMap_NONE, eVRML1Map_NONE )
		( kEnumMap_POINT, eVRML1Map_POINT )
		( kMaskParts_SIDES, eVRML1Parts_SIDES )
		( kMaskParts_TOP, eVRML1Parts_TOP )
		( kMaskParts_BOTTOM, eVRML1Parts_BOTTOM )
		( kMaskParts_ALL, eVRML1Parts_ALL )
		//( kMaskStyle_NONE, eVRML1Style_NONE ) same values as kEnumMap_NONE
		( kMaskStyle_BOLD, eVRML1Style_BOLD )
		( kMaskStyle_ITALIC, eVRML1Style_ITALIC )
		;
	
	mBools.add
		( "FALSE", 0 )
		( "TRUE", 1 )
		;
}

void	SParseState::ConvertNumericArray( bool inConvertSingleToFloat )
{
	::ConvertNumericArray( mProgressStack.back(), inConvertSingleToFloat, mStream );
}

#pragma mark === Action definitions ===
void	PushString::operator()( const char* inStart, const char* inEnd ) const
{
	std::string	theMatch( inStart, inEnd );
	PolyValue	strValue( theMatch );
	mState.mProgressStack.push_back( strValue );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed string " <<
			theMatch << "." << std::endl;
	}
}

void	PushQuotedString::operator()( const char* inStart, const char* inEnd ) const
{
	std::string	theMatch( inStart+1, inEnd-1 );
	UnBackslashEscape( theMatch );
	PolyValue	strValue( theMatch );
	mState.mProgressStack.push_back( strValue );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed string " <<
			theMatch << "." << std::endl;
	}
}

void	PushBool::operator()( int inValue ) const
{
	PolyValue	boolValue( inValue != 0 );
	mState.mProgressStack.push_back( boolValue );

	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed Boolean " <<
			(inValue? "TRUE" : "FALSE") << "." << std::endl;
	}
}

void	PushNULL::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	mState.mProgressStack.push_back( PolyValue() );

	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed NULL." << std::endl;
	}
}

void	StartNode::operator()( char inChar ) const
{
#pragma unused( inChar )
	PolyValue	nodeTypeValue = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	PolyValue	theNode;
	theNode.SetType( PolyValue::kDataTypeDictionary );
	theNode.GetDictionary().insert(
		PolyValue::Dictionary::value_type( std::string("[type]"), nodeTypeValue ) );
	mState.mProgressStack.push_back( theNode );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed " << nodeTypeValue.GetString() << " node." << std::endl;
	}
}

void	StartArray::operator()( char inChar ) const
{
#pragma unused( inChar )
	PolyValue	theArray;
	theArray.SetType( PolyValue::kDataTypeArray );
	mState.mProgressStack.push_back( theArray );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed empty array." << std::endl;
	}
}

void	FinishArray::operator()( char inChar ) const
{
#pragma unused( inChar )
	mState.ConvertNumericArray( false );
}

void	FinishStringArray::operator()( char inChar ) const
{
#pragma unused( inChar )
	PolyValue	theArrayValue;
	theArrayValue.SetType( PolyValue::kDataTypeArray );
	PolyValue::PolyVec&	theArray( theArrayValue.GetPolyVec() );
	
	while ( (not mState.mProgressStack.empty()) and
		(mState.mProgressStack.back().GetType() == PolyValue::kDataTypeString) )
	{
		theArray.push_back( mState.mProgressStack.back() );
		mState.mProgressStack.pop_back();
	}
	
	std::reverse( theArray.begin(), theArray.end() );
	
	if ( (not mState.mProgressStack.empty()) and
		(mState.mProgressStack.back().GetType() == PolyValue::kDataTypeArray) )
	{
		mState.mProgressStack.back() = theArrayValue;
	}
}

void	FinishUnbracketedArray::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	mState.ConvertNumericArray( true );
}

void	FinishNodeChildren::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	if (mState.mProgressStack.back().GetType() == PolyValue::kDataTypeArray)
	{
		PolyValue	nodeArray( mState.mProgressStack.back() );
		mState.mProgressStack.pop_back();
		
		if (mState.mProgressStack.back().GetType() == PolyValue::kDataTypeDictionary)
		{
			PolyValue::Dictionary&	nodeDict( mState.mProgressStack.back().GetDictionary() );
			nodeDict[ "children" ] = nodeArray;
			
			if ( mState.mStream != NULL )
			{
				*mState.mStream << "Added field children to node." << std::endl;
			}
		}
	}
}

void	AppendFloatToPolyArray::operator()( double inValue ) const
{
	if (mState.mProgressStack.back().GetType() != PolyValue::kDataTypeArray)
	{
		PolyValue	newArray;
		newArray.SetType( PolyValue::kDataTypeArray );
		mState.mProgressStack.push_back( newArray );
		
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Start unbracketed number list." << std::endl;
		}
	}
	PolyValue::PolyVec&	theArray( mState.mProgressStack.back().GetPolyVec() );
	theArray.push_back( PolyValue(inValue) );
	
	#if 0
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Added float " << inValue << " to poly array." << std::endl;
	}
	#endif
}

void	AppendIntToPolyArray::operator()( int inValue ) const
{
	if (mState.mProgressStack.back().GetType() != PolyValue::kDataTypeArray)
	{
		PolyValue	newArray;
		newArray.SetType( PolyValue::kDataTypeArray );
		mState.mProgressStack.push_back( newArray );
		
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Start unbracketed number list." << std::endl;
		}
	}
	PolyValue::PolyVec&	theArray( mState.mProgressStack.back().GetPolyVec() );
	theArray.push_back( PolyValue(inValue) );
	
	#if 0
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Added int " << inValue << " to poly array." << std::endl;
	}
	#endif
}

void	AppendNodeToArray::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	// Pop the node
	PolyValue	theNode = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	
	// The top of the stack should now be a PolyVec.  Append the node to it.
	if (mState.mProgressStack.back().GetType() != PolyValue::kDataTypeArray)
	{
		PolyValue	newArray;
		newArray.SetType( PolyValue::kDataTypeArray );
		mState.mProgressStack.push_back( newArray );
		
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Start node array." << std::endl;
		}
	}
	PolyValue::PolyVec&	theArray( mState.mProgressStack.back().GetPolyVec() );
	theArray.push_back( theNode );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Appended node to array." << std::endl;
	}
}

void	AppendQuotedStringToArray::operator()( const char* inStart, const char* inEnd ) const
{
	std::string	theMatch( inStart+1, inEnd-1 );
	UnBackslashEscape( theMatch );
	PolyValue	strValue( theMatch );
	
	// The top of the stack should now be a PolyVec.  Append the string to it.
	PolyValue::PolyVec&	theArray( mState.mProgressStack.back().GetPolyVec() );
	theArray.push_back( strValue );
}


void	FinishDEF::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	// Pop the node name and node
	PolyValue	theNode = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	PolyValue	nodeNameValue = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	
	// Add the name to the node
	theNode.GetDictionary()["[name]"] = nodeNameValue;
	
	// Push the node back on the stack
	mState.mProgressStack.push_back( theNode );
	// Add the named node to the dictionary
	mState.mNamedNodes.insert(
		PolyValue::Dictionary::value_type( nodeNameValue.GetString(), theNode ) );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Recorded named node " << nodeNameValue.GetString() << "." << std::endl;
	}
}

void	FinishUSE::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	// Pop the node name
	PolyValue	nodeNameValue = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	const std::string&	nodeName( nodeNameValue.GetString() );
	// Look up the node
	PolyValue	theNode( mState.mNamedNodes[ nodeName ] );
	// Push the node on the stack
	mState.mProgressStack.push_back( theNode );
	
	if (theNode.GetType() == PolyValue::kDataTypeDictionary)
	{
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Used named node " << nodeName << "." << std::endl;
		}
	}
	else
	{
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Used undefined named node " << nodeName << "." << std::endl;
		}
	}
}

void	FinishTopNode::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	// Pop the node.
	PolyValue	theNode = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	// Record it.
	mState.mCompleteTopNodes.push_back( theNode );

	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Recorded top-level node." << std::endl;
	}
}

void	FinishField::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	// Pop the value.
	PolyValue	theValue = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	// Pop the field name.
	PolyValue	theName = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	const std::string&	nameStr( theName.GetString() );
	// Now the item on the top of the stack should be a dictionary.
	PolyValue::Dictionary&	nodeDict( mState.mProgressStack.back().GetDictionary() );
	nodeDict[ nameStr ] = theValue;

	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Added field " << nameStr << " to node." << std::endl;
	}
}

void	StartMask::operator()( int inValue ) const
{
	PolyValue	theArray;
	theArray.SetType( PolyValue::kDataTypeArrayOfInt );
	theArray.GetIntVec().push_back( inValue );
	mState.mProgressStack.push_back( theArray );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed mask array, first item " << inValue << std::endl;
	}
}

void	MoreMask::operator()( int inValue ) const
{
	PolyValue::IntVec&	theArray( mState.mProgressStack.back().GetIntVec() );
	theArray.push_back( inValue );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Added int " << inValue << " to mask array." << std::endl;
	}
}

void	FinishMask::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	int	theMask = 0;
	PolyValue::IntVec&	theArray( mState.mProgressStack.back().GetIntVec() );
	for (PolyValue::IntVec::iterator i = theArray.begin(); i != theArray.end(); ++i)
	{
		theMask |= *i;
	}
	PolyValue	theValue( theMask );
	mState.mProgressStack.pop_back();
	mState.mProgressStack.push_back( theValue );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Replaced mask array by int value " << theMask << "." << std::endl;
	}
}

#pragma mark -
template <typename ScannerT>
VRMLParser::definition<ScannerT>::definition( const VRMLParser& self )
{
	chset<> nodeTypeFirstChar("A-Z");
	
	chset<> fieldNameFirstChar("a-z");
	chset<>	fieldRestExcluded("{}[]'\"#\\+.&");
	chset<>	sevenBitPrintable("!-~");
	chset<>	fieldRestChar( sevenBitPrintable - fieldRestExcluded );
	
	chset<>	nodeNameExcluded("'\"#\\{}+.");
	chset<>	nodeNameRestChar( sevenBitPrintable - nodeNameExcluded );
	chset<>	digitChars("0123456789");
	chset<>	nodeNameFirstChar( nodeNameRestChar - digitChars );
	
	fieldID
		=	lexeme_d
			[
				fieldNameFirstChar
				>> *fieldRestChar
			][ PushString(self.mState) ];
	
	nodeNameID
		=	lexeme_d
			[
				nodeNameFirstChar
				>> *nodeNameRestChar
			][ PushString(self.mState) ];
	
	nodeTypeID
		=	lexeme_d
			[
				nodeTypeFirstChar
				>> *fieldRestChar
			][ PushString(self.mState) ];
	
	sfBoolValue
		=	self.mState.mBools [ PushBool(self.mState) ];
	
	numArrayMember
		=	strict_real_p[ AppendFloatToPolyArray(self.mState) ]
		|	lexeme_d
			[
				str_p("0x")
				>> hex_p[ AppendIntToPolyArray(self.mState) ]
			]
		|	int_p[ AppendIntToPolyArray(self.mState) ];
	
	// List of unbracketed numbers: This covers several parts of the VRML 2 grammar:
	// sffloatValue, sfcolorValue, sfrotationValue, sfvec2fValue, sfvec3fValue,
	// sfImage
	numArrayValue
		=	(numArrayMember
			>> *(
					numArrayMember
				)
			)[ FinishUnbracketedArray(self.mState) ];
	
	nodeDeclaration
		=	(str_p("DEF") >> nodeNameID >> node)[ FinishDEF(self.mState) ]
		|	(str_p("USE") >> nodeNameID)[ FinishUSE(self.mState) ]
		|	node;
	
	node
		=	nodeTypeID
			>> ch_p('{')[ StartNode( self.mState ) ]
			>> nodeGuts
			>> ch_p('}');
	
	sfNodeValue
		=	str_p("NULL")[ PushNULL(self.mState) ]
		|	nodeDeclaration;
	
	arrayValue
		=	ch_p('[') [ StartArray(self.mState) ]
			>>	*(
					numArrayMember
				|	sfStringValue[ AppendQuotedStringToArray(self.mState) ]
				|	nodeDeclaration[ AppendNodeToArray(self.mState) ]
				)
			>> ch_p(']') [ FinishArray(self.mState) ];
	
	sfStringValue
		=	lexeme_d
			[
				ch_p('"')
				>>	*(
						('\\' >> anychar_p)
					|	~ch_p('"')
					)
				>> ch_p('"')
			];
	
	enumOrMask
		=	(self.mState.mConstants [ StartMask(self.mState) ]
			>>	*(
					'|'
					>> self.mState.mConstants[ MoreMask(self.mState) ]
				)
			) [ FinishMask(self.mState) ]
		;
	
	fieldValue
		=	sfBoolValue
		|	numArrayValue
		|	enumOrMask
		|	sfNodeValue
		|	sfStringValue[ PushQuotedString(self.mState) ]
		|	arrayValue
		;

	nodeGut
		=	(
				str_p("fields")[ PushString(self.mState) ]
				>> ch_p('[')[ StartArray(self.mState) ]
				>> *(
						nodeTypeID
						>> fieldID
					)
				>> ch_p(']')[ FinishStringArray(self.mState) ]
			)[ FinishField(self.mState) ]
		|	(
				fieldID
				>> fieldValue
			)[ FinishField(self.mState) ];
	
	nodeGuts
		=	*nodeGut
			>> (
				*(
					nodeDeclaration[ AppendNodeToArray(self.mState) ]
				)
			)[ FinishNodeChildren(self.mState) ];
	
#if 0
	startRule = nodeGut;
#else
	startRule
		=	*(
				nodeDeclaration[ FinishTopNode(self.mState) ]
			);
#endif
}


/*!
	@function	ParseVRML1
	
	@abstract	Parse VRML, producing an array of nodes in PolyValue dictionary format.
	
	@discussion	This first phase of VRML parsing does not know much about VRML
				semantics.  It does not distinguish between VRML 1 and VRML 2,
				and does not know the differences between specific node types.
				
				In order to avoid backtracking, the grammar allows some things
				that are not allowed in VRML.  For instance, it allows an array
				to contain any mix of values, whereas VRML really only allows
				an array of numbers, an array of strings, or an array of nodes.

	@param		inVRMLText		The VRML input data.
	@param		outNodes		Receives an array of dictionaries.
	@param		ioDebugStream	Receives text describing the parsing process.
								You may pass NULL if you do not want this.
*/
bool	ParseVRML1( const std::string& inVRMLText, PolyValue::PolyVec& outNodes,
				std::ostream* ioDebugStream )
{
	VRML_reader::Skipper	mySkipper;
	SParseState	theState( outNodes, ioDebugStream );
	const char*	textBegin = &*inVRMLText.begin();
	const char*	textEnd = &*inVRMLText.end();
		// The funny &* things are to convert std::string::const_iterator to
		// const char*.
	
	parse_info<>	parseResult = parse(
		textBegin, textEnd,
		theState.mGrammar, mySkipper );
	
	if (ioDebugStream != NULL)
	{
		if (parseResult.full)
		{
			*ioDebugStream << std::endl << "Parsed all input!" << std::endl;
		}
		else
		{
			std::string::size_type	stopOff = parseResult.stop - inVRMLText.data();
			
			if (parseResult.hit)
			{
				*ioDebugStream << std::endl << "Parsed part of input, up to offset " <<
					stopOff << "." << std::endl;
			}
			else
			{
				*ioDebugStream << std::endl << "Failed to parse input, stopped at offset " <<
					stopOff << "." << std::endl;
			}
		}
	}
	
	return parseResult.full;
}
