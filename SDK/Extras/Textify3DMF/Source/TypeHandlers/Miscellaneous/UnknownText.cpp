/*
 *  Normal.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/21/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "UnknownText.h"

UnknownText::UnknownText()
  : TypeHandler( 'uktx', "UnknownText" )
{
}

static std::string QuoteStringFor3DMF( const std::string& inStr )
{
  std::string result( 1, '"' );
  
  for (std::string::const_iterator i = inStr.begin(); i != inStr.end(); ++i)
  {
    char theChar( *i );
    
    switch (theChar)
    {
      case '"':
        result += "\\\"";
        break;
      
      case '\t':
        result += "\\t";
        break;
      
      case '\r':
        result += "\\r";
        break;
      
      case '\n':
        result += "\\n";
        break;
      
      case '\\':
        result += "\\\\";
        break;
      
      default:
        result += theChar;
        break;
    }
  }
  
  result += '"';
  
  return result;
}

void  UnknownText::Process( size_t inStartOffset,
                  size_t inEndOffset )
{
  if (inEndOffset - inStartOffset < 4)
  {
    throw DataLengthException( Name(), inStartOffset, inEndOffset, 4 );
  }

  std::string asciiName;
  std::string contents;
  size_t nameBytesConsumed;
  size_t contentsBytesConsumed;
  bool didRead = Boss()->FetchString( inStartOffset, inEndOffset, asciiName,
                                     nameBytesConsumed );

  if (didRead)
  {
    didRead = Boss()->FetchString( inStartOffset + nameBytesConsumed, inEndOffset, contents,
                                  contentsBytesConsumed );
  }
  
  if ( (not didRead) or (nameBytesConsumed + contentsBytesConsumed < inEndOffset - inStartOffset) )
  {
    throw DataLengthException( Name(), inStartOffset, inEndOffset,
      inEndOffset - inStartOffset );
  }
  
  Out() << Indent() << Name() << " (\n";
  
  Out() << Indent(1) << QuoteStringFor3DMF( asciiName ) << "\n";
  Out() << Indent(1) << QuoteStringFor3DMF( contents ) << "\n";
  
  Out() << Indent() << ")\n";
}
