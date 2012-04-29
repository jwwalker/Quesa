/*
 *  Read3DMF.h
 *  Binify3DMF
 *
 *  Created by James Walker on 4/22/12.
 *
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

#include <vector>
#include <CQ3ObjectRef.h>


/*!
	@function	Read3DMF
	
	@abstract	Read a 3DMF file, producing one or more Quesa objects.
	
	@param		inPath		File system path to a 3DMF file.
	@param		outObjects	Receives objects.
	@param		outText		Receives a flag indicating whether the 3DMF data was
							in text form.
	@result		True if the reading was successful.
*/
bool Read3DMF( const char* inPath,
				std::vector<CQ3ObjectRef>& outObjects,
				bool& outText );
