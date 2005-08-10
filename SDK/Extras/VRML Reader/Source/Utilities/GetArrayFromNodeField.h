#pragma once
/*  NAME:
        GetArrayFromNodeField.h

    DESCRIPTION:
        Template functions to help get arrays of numbers.

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
#include "PolyValue.h"

#if __MACH__
	#include <Quesa/Quesa.h>
#else
	#include <Quesa.h>
#endif


namespace GetNumbersFromIterator
{
	template <class T, class Iter>
	struct GetNums
	{
	};
	
	template <class Iter>
	struct GetNums<TQ3Point3D, Iter>
	{
		void	operator()( Iter& ioIter,
							TQ3Point3D& outPt )
		{
			outPt.x = *ioIter; ++ioIter;
			outPt.y = *ioIter; ++ioIter;
			outPt.z = *ioIter; ++ioIter;
		}
	};

	template <class Iter>
	struct GetNums<TQ3Vector3D, Iter>
	{
		void	operator()( Iter& ioIter,
							TQ3Vector3D& outVec )
		{
			outVec.x = *ioIter; ++ioIter;
			outVec.y = *ioIter; ++ioIter;
			outVec.z = *ioIter; ++ioIter;
		}
	};

	template <class Iter>
	struct GetNums<TQ3ColorRGB, Iter>
	{
		void	operator()( Iter& ioIter,
								TQ3ColorRGB& outColor )
		{
			outColor.r = *ioIter; ++ioIter;
			outColor.g = *ioIter; ++ioIter;
			outColor.b = *ioIter; ++ioIter;
		}
	};
	

	template <class Iter>
	struct GetNums<TQ3Param2D, Iter>
	{
		void	operator()( Iter& ioIter,
								TQ3Param2D& outUV )
		{
			outUV.u = *ioIter; ++ioIter;
			outUV.v = *ioIter; ++ioIter;
		}
	};
}

/*!
	@function	FloatVecToStructureVec
	
	@abstract	Convert a vector of floats to a vector of structures holding
				floats such as TQ3Point3D.
	
	@param		inFloats		A vector of floats.
	@param		outArray		Receives vector of structures.
*/
template <typename T>
void FloatVecToStructureVec( const PolyValue::FloatVec& inFloats,
							std::vector<T>& outArray )
{
	GetNumbersFromIterator::GetNums<T,
		PolyValue::FloatVec::const_iterator>	getter;
	
	for (PolyValue::FloatVec::const_iterator i = inFloats.begin();
		i != inFloats.end(); )	// look Ma, no ++i
	{
		T thing;
		getter( i, thing );
		outArray.push_back( thing );
	}
}

/*!
	@function	GetNodeArray
	
	@abstract	Given a field whose value is a node containing a field that is
				an array of numbers, put those numbers into a vector of
				structures.
	
	@discussion	The most common example is that several node types contain a
				"coord" field whose value must be a "Coordinate" node, which
				has a "point" field that is an array of numbers.  In that case,
				we call this function like so:
				
				std::vector<TQ3Point3D>		thePoints;
				GetNodeArray( dict, "coord", "Coordinate", "point", thePoints );
	
	@param		inDict				The dictionary of a node.
	@param		inFieldName			A field whose value should be a node.
	@param		inNodeType			The expected type of the node field.
	@param		inNodeFieldName		The name of the field in the node that
									contains an array of numbers.
	@param		outArray			Receives vector of structures.
*/
template<typename T>
void GetNodeArray( PolyValue::Dictionary& inDict, const char* inFieldName,
						const char* inNodeType, const char* inNodeFieldName,
						std::vector<T>& outArray )
{
	if (IsKeyPresent( inDict, inFieldName ))
	{
		PolyValue&	theNode( inDict[ inFieldName ] );
		if (theNode.GetType() == PolyValue::kDataTypeDictionary)
		{
			if (theNode.GetDictionary()["[type]"].GetString() == inNodeType)
			{
				PolyValue&	innerValue( theNode.GetDictionary()[inNodeFieldName] );
				if (innerValue.IsNumberVec())
				{
					PolyValue::FloatVec&	theNums( innerValue.GetFloatVec() );
					
					FloatVecToStructureVec( theNums, outArray );
				}
			}
		}
	}
}

