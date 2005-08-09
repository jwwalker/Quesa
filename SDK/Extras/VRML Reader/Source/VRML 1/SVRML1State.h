#pragma once
/*  NAME:
        SVRML1State.h

    DESCRIPTION:
        Header file for SVRML1State.cp.

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

#if __MACH__
	#include <Quesa/Quesa.h>
#else
	#include <Quesa.h>
#endif
#include <vector>

struct SVRML1State
{
					SVRML1State();
					SVRML1State( const SVRML1State& inOther );
	
	void			swap( SVRML1State& ioOther );
	
	SVRML1State&	operator=( const SVRML1State& inOther );
	
	std::vector<TQ3Point3D>		coordinate3;
	std::vector<TQ3Vector3D>	normal;
	std::vector<TQ3Param2D>		textureCoordinate2;
	
	// Material
	std::vector<TQ3ColorRGB>	diffuseColor;
	std::vector<TQ3ColorRGB>	specularColor;
	std::vector<TQ3ColorRGB>	emissiveColor;
	std::vector<double>			shininess;
	std::vector<double>			alpha;
	
	// Binding
	int							materialBinding;
	int							normalBinding;
	
	// Shape hints
	int							vertexOrdering;
	float						creaseAngle;
	bool						solidShapeType;
	bool						convexFaceType;
};
