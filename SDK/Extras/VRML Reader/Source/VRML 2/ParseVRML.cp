/*  NAME:
        ParseVRML.cp

    DESCRIPTION:
        Parse VRML 2 grammar.

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
#include "ParseVRML.h"

#include "ConvertNumericArray.h"
#include "UnBackslashEscape.h"
#include "VRMLSkipParser.h"

#include <ostream>


using namespace boost::spirit;


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
			rule<ScannerT>	identifier;
			rule<ScannerT>	sfBoolValue, numArrayValue;
			rule<ScannerT>	sfNodeValue, node, nodeDeclaration;
			rule<ScannerT>	fieldValue;
			rule<ScannerT>	numArrayMember, intArrayMember;
			rule<ScannerT>	arrayValue;
			rule<ScannerT>	nodeGut, nodeGuts;
			rule<ScannerT>	sfStringValue;
			rule<ScannerT>	routeDeclaration, protoDeclaration, interfaceDeclaration;
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
		
		void					StartIgnoring();
		void					StopIgnoring();
		bool					IsIgnoring() const;
		
		VRMLParser				mGrammar;
		symbols<int>			mBools;
		std::ostream*			mStream;
		PolyValue::PolyVec		mProgressStack;
		PolyValue::PolyVec&		mCompleteTopNodes;
		PolyValue::Dictionary	mNamedNodes;
		long					mIsIgnoringDepth;
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
	DECLARE_NORMAL_ACTION( FinishUnbracketedArray );
	DECLARE_NORMAL_ACTION( AppendQuotedStringToArray );
	DECLARE_NORMAL_ACTION( FinishRoute );
	DECLARE_NORMAL_ACTION( StartIgnoring );
	DECLARE_NORMAL_ACTION( FinishIgnoring );
	
	DECLARE_CHAR_ACTION( StartNode );
	DECLARE_CHAR_ACTION( StartArray );
	DECLARE_CHAR_ACTION( FinishArray );
	
	DECLARE_FLOAT_ACTION( AppendFloatToPolyArray );
	
	DECLARE_INT_ACTION( PushBool );
	DECLARE_INT_ACTION( AppendIntToPolyArray );
	DECLARE_INT_ACTION( AppendIntToIntArray );
}

SParseState::SParseState( PolyValue::PolyVec& ioTopNodes, std::ostream* ioStream )
	: mGrammar( *this ),
	mStream( ioStream ),
	mCompleteTopNodes( ioTopNodes ),
	mIsIgnoringDepth( 0 )
{
	mBools.add
		( "FALSE", 0 )
		( "TRUE", 1 )
		;
}

void	SParseState::ConvertNumericArray( bool inConvertSingleToFloat )
{
	::ConvertNumericArray( mProgressStack.back(), inConvertSingleToFloat, mStream );
}

void	SParseState::StartIgnoring()
{
	mIsIgnoringDepth += 1;
}

void	SParseState::StopIgnoring()
{
	mIsIgnoringDepth -= 1;
}

bool	SParseState::IsIgnoring() const
{
	return (mIsIgnoringDepth > 0);
}

#pragma mark === Action definitions ===
void	PushString::operator()( const char* inStart, const char* inEnd ) const
{
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
	mState.mProgressStack.push_back( PolyValue() );

	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Pushed NULL." << std::endl;
	}
}

void	StartNode::operator()( char inChar ) const
{
#pragma unused( inChar )
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
	mState.ConvertNumericArray( false );
}

void	FinishUnbracketedArray::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	if (mState.IsIgnoring())
	{
		return;
	}
	mState.ConvertNumericArray( true );
}


void	AppendFloatToPolyArray::operator()( double inValue ) const
{
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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

void	AppendIntToIntArray::operator()( int inValue ) const
{
	if (mState.IsIgnoring())
	{
		return;
	}
	if (mState.mProgressStack.back().GetType() != PolyValue::kDataTypeArrayOfInt)
	{
		PolyValue	newArray;
		newArray.SetType( PolyValue::kDataTypeArrayOfInt );
		mState.mProgressStack.push_back( newArray );
		
		if ( mState.mStream != NULL )
		{
			*mState.mStream << "Start unbracketed integer list." << std::endl;
		}
	}

	PolyValue::IntVec&	theArray( mState.mProgressStack.back().GetIntVec() );
	theArray.push_back( inValue );
}

void	AppendNodeToArray::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	if (mState.IsIgnoring())
	{
		return;
	}
	// Pop the node
	PolyValue	theNode = mState.mProgressStack.back();
	mState.mProgressStack.pop_back();
	// The top of the stack should now be a PolyVec.  Append the node to it.
	PolyValue::PolyVec&	theArray( mState.mProgressStack.back().GetPolyVec() );
	theArray.push_back( theNode );
	
	if ( mState.mStream != NULL )
	{
		*mState.mStream << "Appended node to array." << std::endl;
	}
}


void	AppendQuotedStringToArray::operator()( const char* inStart, const char* inEnd ) const
{
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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
	if (mState.IsIgnoring())
	{
		return;
	}
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

void	FinishRoute::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	if (mState.IsIgnoring())
	{
		return;
	}
	// For now we do nothing with ROUTE, just throw away the 4 identifier strings.
	mState.mProgressStack.pop_back();
	mState.mProgressStack.pop_back();
	mState.mProgressStack.pop_back();
	mState.mProgressStack.pop_back();
}

void	StartIgnoring::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	mState.StartIgnoring();
}

void	FinishIgnoring::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	mState.StopIgnoring();
}

void	FinishField::operator()( const char* inStart, const char* inEnd ) const
{
#pragma unused( inStart, inEnd )
	if (mState.IsIgnoring())
	{
		return;
	}
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

#pragma mark -
template <typename ScannerT>
VRMLParser::definition<ScannerT>::definition( const VRMLParser& self )
{
	chset<> notIdFirstChar1("0-9#,.[]\\{}'\" ");
	chset<> notIdFirstChar( notIdFirstChar1 | range_p('\x00', '\x20') );
	chset<> idFirstChar( anychar_p - notIdFirstChar );
	chset<> notIdRestChar1("#,.[]\\{}'\" ");
	chset<> notIdRestChar( notIdRestChar1 | range_p('\x00', '\x20') );
	chset<> idRestChars( anychar_p - notIdRestChar );
	
	identifier
		=	lexeme_d
			[
				idFirstChar
				>> *idRestChars
			][ PushString(self.mState) ];

	sfBoolValue
		=	self.mState.mBools [ PushBool(self.mState) ];
	
	numArrayMember
		=	lexeme_d
			[
				str_p("0x")
				>> hex_p[ AppendIntToPolyArray(self.mState) ]
			]
		|	strict_real_p[ AppendFloatToPolyArray(self.mState) ]
		|	int_p[ AppendIntToPolyArray(self.mState) ];
	
	// List of unbracketed numbers: This covers several parts of the VRML 2 grammar:
	// sffloatValue, sfcolorValue, sfrotationValue, sfvec2fValue, sfvec3fValue,
	// sfImage
	numArrayValue
		=	(
				+numArrayMember
			)[ FinishUnbracketedArray(self.mState) ];
	
	// Special case of integers for image fields, because PixelTexture nodes
	// tend to be very large.
	intArrayMember
		=	lexeme_d
			[
				str_p("0x")
				>> hex_p[ AppendIntToIntArray(self.mState) ]
			]
		|	int_p[ AppendIntToIntArray(self.mState) ];
	
	nodeDeclaration
		=	(str_p("DEF") >> identifier >> node)[ FinishDEF(self.mState) ]
		|	(str_p("USE") >> identifier)[ FinishUSE(self.mState) ]
		|	node;
	
	node
		=	identifier
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
	
	fieldValue
		=	sfBoolValue
		|	numArrayValue
		|	sfNodeValue
		|	sfStringValue[ PushQuotedString(self.mState) ]
		|	arrayValue
		;

		// Handling the image field as a special case is not required by the
		// grammar, it is an optimization.
	nodeGut
		=	(
				(
					str_p("image")[ PushString(self.mState) ]
					>> +intArrayMember
				)
			|	(
					identifier
					>> fieldValue
				)
			)[ FinishField(self.mState) ];
	
	nodeGuts = *nodeGut;
	
	routeDeclaration
		=	str_p("ROUTE")
			>> identifier
			>> ch_p('.')
			>> identifier
			>> str_p("TO")
			>> identifier
			>> ch_p('.')
			>> identifier;

	interfaceDeclaration
		=	(
				(str_p("eventIn") >> identifier >> identifier)
			|	(str_p("eventOut") >> identifier >> identifier)
			|	(str_p("field") >> identifier >> identifier >> fieldValue)
			|	(str_p("exposedField") >> identifier >> identifier >> fieldValue)
			);

	protoDeclaration
		=	str_p("PROTO") [ StartIgnoring(self.mState) ]
			>> identifier
			>> ch_p('[')
			>>	*(interfaceDeclaration)
			>> ch_p(']')
			>> ch_p('{')
			>> *node
			>> ch_p('}');
	
	startRule
		=	*(
				routeDeclaration[ FinishRoute(self.mState) ]
			|	protoDeclaration[ FinishIgnoring(self.mState) ]
			|	nodeDeclaration[ FinishTopNode(self.mState) ]
			)
			>>
			end_p;
}

/*!
	@function	ParseVRML
	
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
bool	ParseVRML( const std::string& inVRMLText, PolyValue::PolyVec& outNodes,
				std::ostream* ioDebugStream )
{
	VRML_reader::Skipper	mySkipper;
	SParseState	theState( outNodes, ioDebugStream );
	
	parse_info<>	parseResult = parse(
		&*inVRMLText.begin(), &*inVRMLText.end(),
		theState.mGrammar, mySkipper );
		// The funny &* things are to convert std::string::const_iterator to
		// const char*.
	
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
