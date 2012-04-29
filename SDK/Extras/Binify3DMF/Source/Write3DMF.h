/*
 *  Write3DMF.h
 *  Binify3DMF
 *
 *  Created by James Walker on 4/24/12.
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
#include <stdio.h>


/*!
	@function	Write3DMF
	
	@abstract	Write Quesa objects to a binary 3DMF stream.
	
	@param		inObjects		Objects to write.
	@param		inBinStream		A stream to write to.
	@result		Success or failure of the operation.
*/
bool	Write3DMF( const std::vector<CQ3ObjectRef>& inObjects,
					FILE* inBinStream );
