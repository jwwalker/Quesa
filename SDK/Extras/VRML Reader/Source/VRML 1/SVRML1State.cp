/*  NAME:
        SVRML1State.cp

    DESCRIPTION:
        Structure used on state stack for VRML 1.

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
#include "SVRML1State.h"

#include "VRML_1_constants.h"

namespace
{
	const TQ3ColorRGB	kDefaultDiffuseColor = { 0.8f, 0.8f, 0.8f };
	const TQ3ColorRGB	kDefaultSpecularColor = { 0.0f, 0.0f, 0.0f };
	const TQ3ColorRGB	kDefaultEmissiveColor = { 0.0f, 0.0f, 0.0f };
	const float			kDefaultShininess	= 0.2f;
	const float			kDefaultAlpha		= 1.0f;
}


SVRML1State::SVRML1State( const SVRML1State& inOther )
	: coordinate3( inOther.coordinate3 ),
	normal( inOther.normal ),
	textureCoordinate2( inOther.textureCoordinate2 ),
	diffuseColor( inOther.diffuseColor ),
	specularColor( inOther.specularColor ),
	emissiveColor( inOther.emissiveColor ),
	shininess( inOther.shininess ),
	alpha( inOther.alpha ),
	materialBinding( inOther.materialBinding ),
	normalBinding( inOther.normalBinding ),
	vertexOrdering( inOther.vertexOrdering ),
	creaseAngle( inOther.creaseAngle ),
	solidShapeType( inOther.solidShapeType ),
	convexFaceType( inOther.convexFaceType )
{
}

SVRML1State::SVRML1State()
	: materialBinding( eVRML1Value_OVERALL ),
	normalBinding( eVRML1Value_DEFAULT ),
	vertexOrdering( eVRML1VertexOrdering_UNKNOWN_ORDERING ),
	creaseAngle( 0.5f ),
	solidShapeType( false ),
	convexFaceType( true )
{
	diffuseColor.push_back( kDefaultDiffuseColor );
	specularColor.push_back( kDefaultSpecularColor );
	emissiveColor.push_back( kDefaultEmissiveColor );
	shininess.push_back( kDefaultShininess );
	alpha.push_back( kDefaultAlpha );
}

void	SVRML1State::swap( SVRML1State& ioOther )
{
	coordinate3.swap( ioOther.coordinate3 );
	normal.swap( ioOther.normal );
	textureCoordinate2.swap( ioOther.textureCoordinate2 );
	diffuseColor.swap( ioOther.diffuseColor );
	specularColor.swap( ioOther.specularColor );
	emissiveColor.swap( ioOther.emissiveColor );
	shininess.swap( ioOther.shininess );
	alpha.swap( ioOther.alpha );
	
	std::swap( materialBinding, ioOther.materialBinding );
	std::swap( normalBinding, ioOther.normalBinding );
	std::swap( vertexOrdering, ioOther.vertexOrdering );
	std::swap( creaseAngle, ioOther.creaseAngle );
	std::swap( solidShapeType, ioOther.solidShapeType );
	std::swap( convexFaceType, ioOther.convexFaceType );
}

SVRML1State&	SVRML1State::operator=( const SVRML1State& inOther )
{
	SVRML1State	temp( inOther );
	swap( temp );
	return *this;
}
