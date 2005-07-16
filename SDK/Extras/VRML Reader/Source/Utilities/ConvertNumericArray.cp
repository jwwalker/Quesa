/*  NAME:
        ConvertNumericArray.cp

    DESCRIPTION:
       Utility function.

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
#include "ConvertNumericArray.h"

#include <ostream>


/*!
	@function	ConvertNumericArray
	
	@abstract	Convert a numeric array to an array of ints or floats if possible.
				Optionally, we may also convert an array containing a single
				number to a plain numeric value rather than an array.
	
	@param		ioArray					A polymorphic value that is presumed to
										be of type kDataTypeArray.
	
	@param		inConvertSingleToFloat	Whether an array containing a single
										number should be converted to a naked
										float.
	
	@param		ioDebugStream			If this is not NULL, text will be
										written to this stream if array
										conversions were performed.
*/
void	ConvertNumericArray( PolyValue& ioArray, bool inConvertSingleToFloat,
							std::ostream* ioDebugStream )
{
	if (ioArray.GetType() == PolyValue::kDataTypeArray)
	{
		PolyValue::PolyVec&	theArray( ioArray.GetPolyVec() );
		
		if ( inConvertSingleToFloat and (theArray.size() == 1) and
			theArray[0].IsNumber() )
		{
			float	theFloat = theArray[0].GetFloat();
			PolyValue	singleFloat( theFloat );
			ioArray = singleFloat;
		}
		else if (not theArray.empty())
		{
			bool	isAllInteger = true;
			bool	isAllFloat = true;
			PolyValue::PolyVec::iterator	i;
			for (i = theArray.begin(); i != theArray.end(); ++i)
			{
				PolyValue::DataType	theType = i->GetType();
				if (theType == PolyValue::kDataTypeFloat)
				{
					isAllInteger = false;
				}
				else if (theType == PolyValue::kDataTypeInt)
				{
					// nothing to do
				}
				else
				{
					isAllInteger = false;
					isAllFloat = false;
				}
			}
			
			if (isAllInteger)
			{
				PolyValue	intArray;
				intArray.SetType( PolyValue::kDataTypeArrayOfInt );
				PolyValue::IntVec&	intVec( intArray.GetIntVec() );
				intVec.reserve( theArray.size() );
				for (i = theArray.begin(); i != theArray.end(); ++i)
				{
					intVec.push_back( i->GetInt() );
				}
				ioArray = intArray;
				if ( ioDebugStream != NULL )
				{
					*ioDebugStream << "Converted poly array to int array." << std::endl;
				}
			}
			else if (isAllFloat)
			{
				PolyValue	floatArray;
				floatArray.SetType( PolyValue::kDataTypeArrayOfFloat );
				PolyValue::FloatVec&	floatVec( floatArray.GetFloatVec() );
				floatVec.reserve( theArray.size() );
				for (i = theArray.begin(); i != theArray.end(); ++i)
				{
					floatVec.push_back( i->GetFloat() );
				}
				ioArray = floatArray;
				if ( ioDebugStream != NULL )
				{
					*ioDebugStream << "Converted poly array to float array." << std::endl;
				}
			}
		}
	}
}
