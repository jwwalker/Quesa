/*  NAME:
       QOGLSLShaders.cpp

    DESCRIPTION:
        Quesa OpenGL shaders.
		    
    COPYRIGHT:
        Copyright (c) 2020, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "QOGLSLShaders.h"

namespace QOGLSLShader
{

#pragma mark kVertexShaderStart
const char* kVertexShaderStart = R"(
#version 150 core
const float pi = 3.14159265358979323846264338327950288;
// Switch for layer shifting
uniform bool isLayerShifting;

// Basic matrices
uniform mat4 quesaModelViewMtx;
uniform mat4 quesaProjectionMtx;
uniform mat4 quesaTextureMtx;
uniform mat3 quesaNormalMtx;

// Other camera info
uniform vec2 quesaCameraRange;		// near and far
uniform vec4 quesaCameraViewport;	// origin.x, origin.y, width, height

// Vertex attributes
in vec4 quesaVertex;
in vec2 quesaTexCoord0;
in vec3 quesaNormal;
in vec4 quesaColor;
in float quesaLayerShift;

// Outputs
struct VertexData {
	// Normal vector in eye coordinates
	vec3 ECNormal;
	// Position in eye coordinates
	vec4 ECPos4;
	// Transformed texture coordinate
	vec2 texCoord;
	// Color
	vec4 interpolatedColor;
};
out VertexData vs_out;

void main()
{
	// Transform normal to eye coordinates, and let it
	// be interpolated across the primitive.
	// There is no point normalizing yet.
	vs_out.ECNormal = quesaNormalMtx * quesaNormal;

	// Transform vertex position to eye coordinates.
	vec4 ECPosition = quesaModelViewMtx * quesaVertex;

	vs_out.texCoord = (quesaTextureMtx * vec4( quesaTexCoord0, 0.0, 0.0 )).st;

	vs_out.interpolatedColor = quesaColor;

	// Standardize the homogeneous coordinates.
	if (ECPosition.w != 0.0) {
		ECPosition.xyz = ECPosition.xyz / ECPosition.w;
		ECPosition.w = 1.0;
		// Layer shift.  Positive layers bring the vertex closer.
		ECPosition.z += 0.00635 * quesaLayerShift * float(isLayerShifting);
	}
	// Note that from now on, ECPosition.w is always exactly 1.0 or 0.0.

	vs_out.ECPos4 = ECPosition;
)";


#pragma mark kVertexShaderStandardProjection
const char* kVertexShaderStandardProjection = R"(
	// Compute position in device coordinates.
	gl_Position = quesaProjectionMtx * vs_out.ECPos4;
)";


#pragma mark kVertexShaderAllSeeingProjection
const char* kVertexShaderAllSeeingProjection = R"(
	float near = quesaCameraRange.x;
	float far = quesaCameraRange.y;
	float q = isinf( far )? 1.0 : far / (far - near);
	float r = length(vs_out.ECPos4.xyz);
	gl_Position.x = atan( vs_out.ECPos4.x, -vs_out.ECPos4.z ) / pi;
	gl_Position.y = (2.0 * asin( vs_out.ECPos4.y / r )) / pi;
	gl_Position.z = (vs_out.ECPos4.w == 0.0)? 2.0 * q - 1.0 : 2.0 * q - 1.0 - (2.0 * q * near) / r;
	gl_Position.w = 1.0;
)";


#pragma mark kVertexShaderEnd
const char* kVertexShaderEnd = "}\n";

#pragma mark -

#pragma mark kAllSeeingGeomShader
/*
	Geometry shader for all-seeing camera.
	One reason for the shader is to excise parts of a triangle
	that hit discontinuities in the projection.  Another is
	to subdivide triangles to reduce inaccuracy caused by the
	nonlinearity of the projection.
	
	The excision phase starts with one triangle and subdivides
	it into at most 4 triangles.  Then the subdivision phase
	breaks each triangle into at most 4 triangles.  Thus, at
	most 16 triangles will be emitted.  Assuming that we simply
	emit each triangle separately rather than using strips,
	this means we may emit up to 16*3 = 48 vertices.  Hence the
	value used for max_vertices.
	
	Note that since GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS has
	a minimum value of 1024, and our VertexData structure has
	13 components, we could emit up to floor(1024/13) = 78
	vertices and expect it to work on every reasonable graphics
	card.
*/
const char* kAllSeeingGeomShader = R"(
#version 150 core
layout (triangles) in;
layout (triangle_strip, max_vertices=48) out;
const float pi = 3.14159265358979323846264338327950288;
const float delta = 0.00001;
const float triBigSize = 0.174;

// Inputs
struct VertexData {
	vec3 ECNormal; // Normal vector in eye coordinates
	vec4 ECPos4;	// Position in eye coordinates
	vec2 texCoord;	// Transformed texture coordinate
	vec4 interpolatedColor;	// Color
};
in VertexData vs_out[];
uniform vec2 quesaCameraRange;
uniform vec4 quesaCameraViewport;	// origin.x, origin.y, width, height

// Outputs
out VertexData gs_out;

// Given two points A, B in eye space, find a parameter p where the
// line segment from A to B crosses the plane x == 0, i.e., where
// the x coordinate of (1-p) A + p B is 0.  Also, return whether the
// crossing point has z >= 0, making it a point of discontinuity of
// the equirectangular projection.
bool SectYZPlane( in vec3 A, in vec3 B, out float p )
{
	bool isBreak = false;
	p = -1.0;
	if (abs(A.x - B.x) > 0.0)
	{
		p = -A.x / (B.x - A.x);
		if ( (p >= 0.0) && (p <= 1.0) )
		{
			vec3 crossing = (1.0-p) * A + p * B;
			isBreak = (crossing.z >= 0.0);
		}
	}
	else if (abs(A.x) < 0.00001)
	{
		isBreak = (A.z >= 0.0) || (B.z >= 0.0);
	}
	
	return isBreak;
}

// Given 3 points A, B, C in eye space, look for a point in the interior of the
// triangle on the y axis, i.e., where x == 0 and z == 0, and return its
// barycentric coordinates.
bool FindPoleInTriangle( in vec3 A, in vec3 B, in vec3 C, out vec3 baryParam )
{
	bool foundPole = false;
	vec3 A_C = A - C;
	vec3 B_C = B - C;
	float denom = A_C.x * B_C.z - B_C.x * A_C.z;
	if (abs(denom) > 0.000001)
	{
		baryParam.x = (-C.x * B_C.z + B_C.x * C.z) / denom;
		baryParam.y = (-A_C.x * C.z + C.x * A_C.z) / denom;
		baryParam.z = 1.0 - baryParam.x - baryParam.y;
		foundPole = (baryParam.x >= 0.0) && (baryParam.y >= 0.0) && (baryParam.z >= 0.0);
	}
	
	return foundPole;
}

vec4 AllSeeingProjection( in VertexData vert )
{
	float near = quesaCameraRange.x;
	float far = quesaCameraRange.y;
	float q = isinf( far )? 1.0 : far / (far - near);
	float r = length(vert.ECPos4.xyz);
	vec4 projected;
	projected.x = atan( vert.ECPos4.x, -vert.ECPos4.z ) / pi;
	projected.y = (2.0 * asin( vert.ECPos4.y / r )) / pi;
	projected.z = (vert.ECPos4.w == 0.0)? 2.0 * q - 1.0 : 2.0 * q - 1.0 - (2.0 * q * near) / r;
	projected.w = 1.0;
	
	// viewport mapping
	projected.x = 2.0*(projected.x - quesaCameraViewport.x)/quesaCameraViewport.z - 1.0;
	projected.y = 2.0*(projected.y - quesaCameraViewport.y)/quesaCameraViewport.w + 1.0;
	
	return projected;
}

void VertexBlend( in float s, in VertexData A, in VertexData B, out VertexData blend )
{
	blend.ECNormal = normalize( (1.0 - s) * A.ECNormal + s * B.ECNormal );
	blend.ECPos4 = (1.0 - s) * A.ECPos4 + s * B.ECPos4;
	// Up to now, either ECPos4 has 0 in the w position, or has 1 in the
	// w position.
	// Let's maintain that condition.
	if ( (blend.ECPos4.w != 1.0) && (blend.ECPos4.w != 0.0) )
	{
		blend.ECPos4.xyz = blend.ECPos4.xyz / blend.ECPos4.w;
		blend.ECPos4.w = 1.0;
	}
	blend.texCoord = (1.0 - s) * A.texCoord + s * B.texCoord;
	blend.interpolatedColor = (1.0 - s) * A.interpolatedColor + s * B.interpolatedColor;
}

void EmitVert( in VertexData v )
{
	gs_out = v;
	gl_Position = AllSeeingProjection( v );
	EmitVertex();
}

void TintVert( in VertexData v, in vec3 color )
{
	gs_out = v;
	gs_out.interpolatedColor.rgb = color;
	gl_Position = AllSeeingProjection( v );
	EmitVertex();
}

float Angle( in vec3 u, in vec3 v )
{
	return atan( length( cross( u, v ) ), dot( u, v ) );
}

void Sub0( in VertexData A, in VertexData B, in VertexData C )
{
	EmitVert( A );
	EmitVert( B );
	EmitVert( C );
	EndPrimitive();
}

void Sub1( in VertexData A, in VertexData B, in VertexData C )
{
	bool isLongAB = Angle( A.ECPos4.xyz, B.ECPos4.xyz ) > triBigSize;
	bool isLongAC = Angle( A.ECPos4.xyz, C.ECPos4.xyz ) > triBigSize;
	bool isLongBC = Angle( B.ECPos4.xyz, C.ECPos4.xyz ) > triBigSize;
	int longCount = int(isLongAB) + int(isLongAC) + int(isLongBC);

	if (longCount == 3)
	{
		VertexData AB, AC, BC;
		VertexBlend( 0.5, A, B, AB );
		VertexBlend( 0.5, A, C, AC );
		VertexBlend( 0.5, B, C, BC );
		Sub0( A, AB, AC );
		Sub0( AB, B, BC );
		Sub0( AC, BC, C );
		Sub0( AB, BC, AC );
	}
	else if (longCount == 0)
	{
		Sub0( A, B, C );
	}
	else if (longCount == 2)
	{
		// Permute so that AB and AC are long, BC is short
		if (! isLongAB)
		{
			VertexData temp = A;
			A = C;
			C = B;
			B = temp;
		}
		else if (!isLongAC)
		{
			VertexData temp = A;
			A = B;
			B = C;
			C = temp;
		}
		VertexData AB, AC;
		VertexBlend( 0.5, A, B, AB );
		VertexBlend( 0.5, A, C, AC );
		Sub0( A, AB, AC );
		Sub0( B, AC, AB );
		Sub0( B, C, AC );
	}
	else if (longCount == 1)
	{
		// Permute so that BC is long
		if (isLongAB)
		{
			VertexData temp = A;
			A = C;
			C = B;
			B = temp;
		}
		else if (isLongAC)
		{
			VertexData temp = A;
			A = B;
			B = C;
			C = temp;
		}
		VertexData BC;
		VertexBlend( 0.5, B, C, BC );
		Sub0( A, B, BC );
		Sub0( A, BC, C );
	}
}


void main()
{
	// Excision of discontinuities
	float p01, p02, p12;
	vec3 v0 = vs_out[0].ECPos4.xyz;
	vec3 v1 = vs_out[1].ECPos4.xyz;
	vec3 v2 = vs_out[2].ECPos4.xyz;
	bool break01 = SectYZPlane( v0, v1, p01 );
	bool break02 = SectYZPlane( v0, v2, p02 );
	bool break12 = SectYZPlane( v1, v2, p12 );
	vec3 poleParam;
	bool hasPole = FindPoleInTriangle( v0, v1, v2, poleParam );
	int breakCount = int(break01) + int(break02) + int(break12);
	if ( breakCount == 0 )  // does not hit the half-plane of discontinuity
	{
		Sub1( vs_out[0], vs_out[1], vs_out[2] );
	}
	else if ( breakCount == 2 ) // 2 edges intersect the half-plane
	{
		VertexData A, B, C;
		float paramAB, paramAC;
		// Rearrange our vertices 0, 1, 2 in the same orientation
		// so that both crossings are adjacent to A, and such that
		// the AB crossing is the point (1-paramAB)*A + paramAB*B
		// and the AC crossing is (1-paramAC)*A + paramAC*C.
		if (! break01) // so there are 02 and 12 crossings
		{
			A = vs_out[2];
			B = vs_out[0];
			C = vs_out[1];
			paramAB = 1.0 - p02;
			paramAC = 1.0 - p12;
		}
		else if (!break02)
		{
			A = vs_out[1];
			B = vs_out[2];
			C = vs_out[0];
			paramAB = p12;
			paramAC = 1.0 - p01;
		}
		else // !break12
		{
			A = vs_out[0];
			B = vs_out[1];
			C = vs_out[2];
			paramAB = p01;
			paramAC = p02;
		}
		VertexData D, E, F, G;
		VertexBlend( paramAB - delta, A, B, D );
		VertexBlend( paramAB + delta, A, B, F );
		VertexBlend( paramAC - delta, A, C, E );
		VertexBlend( paramAC + delta, A, C, G );
		
		Sub1( A, D, E );
		Sub1( C, F, B );
		Sub1( C, G, F );
	}
	else if ( hasPole && (breakCount == 1) )
	{
		// Rearrange our vertices 0, 1, 2 as A, B, C
		// in the same orientation so that the half-plane of
		// discontinuity crosses BC with parameter p,
		// and the pole has barycentric coordinates poleP.
		VertexData A, B, C;
		float p;
		vec3 poleP;
		if (break01)
		{
			A = vs_out[2];
			B = vs_out[0];
			C = vs_out[1];
			p = p01;
			poleP.xyz = poleParam.zxy;
		}
		else if (break02)
		{
			A = vs_out[1];
			B = vs_out[2];
			C = vs_out[0];
			p = 1.0 - p02;
			poleP.xyz = poleParam.yzx;
		}
		else // break12
		{
			A = vs_out[0];
			B = vs_out[1];
			C = vs_out[2];
			p = p12;
			poleP = poleParam;
		}
		
		// Compute a vertex H at the pole.  This requires blending 3 points, while VertexBlend
		// handles 2.  The idea is that you can compute a blend s*A + t*B + u*C as
		// s*A + (1-s)(t/(1-s)*B + u/(1-s)*C) so long as s != 1.
		VertexData H;
		if (poleP.x == 1.0)
		{
			H = A;
		}
		else
		{
			VertexData temp;
			VertexBlend( poleP.z / (1.0 - poleP.x), B, C, temp );
			VertexBlend( 1.0 - poleP.x, A, temp, H );
		}
		// Compute the point G where the half-plane of discontinuity intersects
		// the BC segment.
		VertexData G;
		VertexBlend( p, B, C, G );
		
		// We want a point D on the interior side of H.  Usually we can go a little
		// past H along the line from G thru H.  But if the pole is along the BC
		// segment, then G and H should be the same, so instead go a bit toward A
		// from H.
		VertexData D;
		if (abs(poleP.x) < 0.000001) // pole along BC
		{
			VertexBlend( delta, G, A, D );
		}
		else
		{
			VertexBlend( -delta, H, G, D );
		}
		// Compute points E, F on the sides of G.
		VertexData E, F;
		VertexBlend( p - delta, B, C, E );
		VertexBlend( p + delta, B, C, F );
		
		// Now make 4 triangles that avoid the discontinuity.
		Sub1( A, B, D );
		Sub1( A, D, C );
		Sub1( F, C, D );
		Sub1( B, E, D );
	}
}
)";

#pragma mark kFaceEdgeGeomShader
const char* kFaceEdgeGeomShader = R"(
#version 150 core
layout (triangles) in;
layout (triangle_strip, max_vertices=12) out;

// Uniforms
uniform vec2 viewportSize;
uniform float lineWidth;
uniform bool cullFrontFaces;
uniform bool cullBackFaces;

// Inputs
struct VertexData {
	// Normal vector in eye coordinates
	vec3 ECNormal;
	// Position in eye coordinates
	vec4 ECPos4;
	// Transformed texture coordinate
	vec2 texCoord;
	// Color
	vec4 interpolatedColor;
};
in VertexData vs_out[];

// Outputs
out VertexData gs_out;

// Subroutines
vec3 clipToNDC( in vec4 clipPt ) {
	return (1.0 / clipPt.w) * clipPt.xyz;
}

vec4 ndcToClip( in vec3 ndcPt, in float w ) {
	return vec4( w * ndcPt, w );
}

vec2 ndcToView( in vec3 ndcPt ) {
	return 0.5 * viewportSize * ndcPt.xy;
}

vec3 viewToNDC( in vec2 viewPt ) {
	return vec3( (2.0 / viewportSize) * viewPt, 0.0 );
}

void lineToBox( in vec4 startClip, in vec4 endClip, in bool flip,
				out vec4 startClipPlus, out vec4 startClipMinus,
				out vec4 endClipPlus, out vec4 endClipMinus ) {
	float startW = startClip.w;
	float endW = endClip.w;
	vec3 startNDC = clipToNDC( startClip );
	vec3 endNDC = clipToNDC( endClip );
	vec2 diffView = ndcToView( endNDC - startNDC );
	vec2 perp = normalize(vec2( diffView.y, -diffView.x )) * (flip? -1.0 : 1.0);
	vec3 delta = 0.5 * lineWidth * viewToNDC( perp );
	startClipPlus = ndcToClip( startNDC + delta, startW );
	endClipPlus = ndcToClip( endNDC + delta, endW );
	startClipMinus = ndcToClip( startNDC - delta, startW );
	endClipMinus = ndcToClip( endNDC - delta, endW );
}

void CopyVertex( in float flipSign, in VertexData vsOut, out VertexData gsOut ) {
	gsOut.ECNormal = flipSign * vsOut.ECNormal;
	gsOut.ECPos4 = vsOut.ECPos4;
	gsOut.texCoord = vsOut.texCoord;
	gsOut.interpolatedColor = vsOut.interpolatedColor;
}

void main() {
	// Culling
	vec3 ndc0 = clipToNDC( gl_in[0].gl_Position );
	vec3 ndc1 = clipToNDC( gl_in[1].gl_Position );
	vec3 ndc2 = clipToNDC( gl_in[2].gl_Position );
	vec3 frontVec = cross( ndc1 - ndc0, ndc2 - ndc0 );
	bool facesFront = (frontVec.z > 0.0);
	if ( (cullFrontFaces && facesFront) || (cullBackFaces && (! facesFront) ) ) {
		return;
	}

// When I generate 2D geometry from the edges, it is normally front-facing.  But if
// we are culling front faces, we need to flip the orientation and normals.
	bool flip = cullFrontFaces;
	float normalFlip = facesFront? 1.0 : -1.0;

// I tried doing this in a nicer way than just handling each edge separately,
// but couldn't get it to work, and it didn't seem worth the effort to pursue.
	vec4 startPlusClip, startMinusClip, endPlusClip, endMinusClip;
	lineToBox( gl_in[0].gl_Position, gl_in[1].gl_Position, flip,
				startPlusClip, startMinusClip, endPlusClip, endMinusClip );

	gl_Position = startPlusClip;
	CopyVertex( normalFlip, vs_out[0], gs_out );
	EmitVertex();

	gl_Position = endPlusClip;
	CopyVertex( normalFlip, vs_out[1], gs_out );
	EmitVertex();

	gl_Position = startMinusClip;
	CopyVertex( normalFlip, vs_out[0], gs_out );
	EmitVertex();

	gl_Position = endMinusClip;
	CopyVertex( normalFlip, vs_out[1], gs_out );
	EmitVertex();

	EndPrimitive();

	lineToBox( gl_in[1].gl_Position, gl_in[2].gl_Position, flip,
				startPlusClip, startMinusClip, endPlusClip, endMinusClip );

	gl_Position = startPlusClip;
	CopyVertex( normalFlip, vs_out[1], gs_out );
	EmitVertex();

	gl_Position = endPlusClip;
	CopyVertex( normalFlip, vs_out[2], gs_out );
	EmitVertex();

	gl_Position = startMinusClip;
	CopyVertex( normalFlip, vs_out[1], gs_out );
	EmitVertex();

	gl_Position = endMinusClip;
	CopyVertex( normalFlip, vs_out[2], gs_out );
	EmitVertex();

	EndPrimitive();

	lineToBox( gl_in[2].gl_Position, gl_in[0].gl_Position, flip,
				startPlusClip, startMinusClip, endPlusClip, endMinusClip );

	gl_Position = startPlusClip;
	CopyVertex( normalFlip, vs_out[2], gs_out );
	EmitVertex();

	gl_Position = endPlusClip;
	CopyVertex( normalFlip, vs_out[0], gs_out );
	EmitVertex();

	gl_Position = startMinusClip;
	CopyVertex( normalFlip, vs_out[2], gs_out );
	EmitVertex();

	gl_Position = endMinusClip;
	CopyVertex( normalFlip, vs_out[0], gs_out );
	EmitVertex();

	EndPrimitive();
}
)";


#pragma mark kLineGeomShader
const char* kLineGeomShader = R"(
#version 150 core
layout (lines) in;
layout (triangle_strip, max_vertices=4) out;

// Uniforms
uniform vec2 viewportSize;
uniform float lineWidth;

// Inputs
struct VertexData {
	// Normal vector in eye coordinates
	vec3 ECNormal;
	// Position in eye coordinates
	vec4 ECPos4;
	// Transformed texture coordinate
	vec2 texCoord;
	// Color
	vec4 interpolatedColor;
};
in VertexData vs_out[];

// Outputs
out VertexData gs_out;

// Subroutines
vec3 clipToNDC( in vec4 clipPt ) {
	return (1.0 / clipPt.w) * clipPt.xyz;
}

vec4 ndcToClip( in vec3 ndcPt, in float w ) {
	return vec4( w * ndcPt, w );
}

vec2 ndcToView( in vec3 ndcPt ) {
	return 0.5 * viewportSize * ndcPt.xy;
}

vec3 viewToNDC( in vec2 viewPt ) {
	return vec3( 2.0 / viewportSize * viewPt, 0.0 );
}

void lineToBox( in vec4 startClip, in vec4 endClip,
				out vec4 startClipPlus, out vec4 startClipMinus,
				out vec4 endClipPlus, out vec4 endClipMinus ) {
	float startW = startClip.w;
	float endW = endClip.w;
	vec3 startNDC = clipToNDC( startClip );
	vec3 endNDC = clipToNDC( endClip );
	vec2 diffView = ndcToView( endNDC - startNDC );
	vec2 perp = normalize(vec2( diffView.y, -diffView.x ));
	vec3 delta = 0.5 * lineWidth * viewToNDC( perp );
	startClipPlus = ndcToClip( startNDC + delta, startW );
	endClipPlus = ndcToClip( endNDC + delta, endW );
	startClipMinus = ndcToClip( startNDC - delta, startW );
	endClipMinus = ndcToClip( endNDC - delta, endW );
}

void CopyVertex( in VertexData vsOut, out VertexData gsOut ) {
	gsOut.ECNormal = vsOut.ECNormal;
	gsOut.ECPos4 = vsOut.ECPos4;
	gsOut.texCoord = vsOut.texCoord;
	gsOut.interpolatedColor = vsOut.interpolatedColor;
}

void main() {
	vec4 startPlusClip, startMinusClip, endPlusClip, endMinusClip;
	lineToBox( gl_in[0].gl_Position, gl_in[1].gl_Position,
				startPlusClip, startMinusClip, endPlusClip, endMinusClip );

	gl_Position = startPlusClip;
	CopyVertex( vs_out[0], gs_out );
	EmitVertex();

	gl_Position = endPlusClip;
	CopyVertex( vs_out[1], gs_out );
	EmitVertex();

	gl_Position = startMinusClip;
	CopyVertex( vs_out[0], gs_out );
	EmitVertex();

	gl_Position = endMinusClip;
	CopyVertex( vs_out[1], gs_out );
	EmitVertex();

	EndPrimitive();
}
)";


#pragma mark -

#pragma mark kFragmentShaderPrefix
const char* kFragmentShaderPrefix = R"(
#version 150 core

// Types
struct VertexData {
	// Normal vector in eye coordinates
	vec3 ECNormal;
	// Position in eye coordinates
	vec4 ECPos4;
	// Transformed texture coordinate
	vec2 texCoord;
	// Color
	vec4 interpolatedColor;
};

// Color out
out vec4 fragColor;

// Light quantization parameter
uniform float quantization;

// Cartoon edges parameter
uniform float lightNearEdge;

// Hot angles, cutoff angles for spot lights (in radians)
uniform float hotAngle[gl_MaxLights];
uniform float cutoffAngle[gl_MaxLights];

// Light data, to replace gl_LightSource and gl_LightModel.ambient
uniform vec4 lightPosition[gl_MaxLights];
uniform vec4 lightColor[gl_MaxLights];
uniform vec3 spotLightDirection[gl_MaxLights];
uniform vec3 lightAttenuation[gl_MaxLights];
uniform vec3 ambientLight;

// Specular map flag
uniform bool isUsingSpecularMap;

// Specularity, replacing gl_FrontMaterial.specular.rgb, gl_FrontMaterial.shininess
uniform vec3 specularColor;
uniform float shininess;

// Emissive color, replacing gl_FrontMaterial.emission.rgb
uniform vec3 quesaEmissiveColor;

// Alpha test
uniform float alphaThreshold;

// Flipping normals flag
uniform bool isFlippingNormals;

// Clipping plane
uniform vec4 ECClipPlane;

// Standard fog
uniform vec3 fogColor;
uniform float fogDensity;
uniform float linearFogEnd;
uniform float linearFogScale;

// Extended fog
uniform float maxFogOpacity;
uniform float HSFogRate;
uniform vec4 HSFogPlane;

// Samplers for texture units
uniform sampler2D tex0;
uniform sampler2D tex1;

float FogSmooth( in float x )
{
	float y = x / maxFogOpacity;
	return x * ( y * (1.0 - y) + 1.0 );
}

)";


#pragma mark kFragmentShaderInputWithoutGeomShader
/*
	I ran into a compatibility problem with interface blocks,
	so I do not use them.  But this means that the name of
	the input to the fragment shader varies depending on
	whether there was a geometry shader.
 */
const char* kFragmentShaderInputWithoutGeomShader = R"(
#define FSIN vs_out
in VertexData FSIN;
)";


#pragma mark kFragmentShaderInputWithGeomShader
const char* kFragmentShaderInputWithGeomShader = R"(
#define FSIN gs_out
in VertexData FSIN;
)";


#pragma mark kFragmentShaderQuantizeFuncs_Normal
const char*	kFragmentShaderQuantizeFuncs_Normal = R"(
vec3 QuantizeLight( in vec3 light )
{
	return light;
}

float QuantizeDot( in float normalDotGeomToLight )
{
	return normalDotGeomToLight;
}

vec3 QuantizeDiffuse( in vec3 light, in float normalDotGeomToLight )
{
	return light * normalDotGeomToLight;
}

)";


#pragma mark kFragmentShaderQuantizeFuncs_Cartoonish
const char* kFragmentShaderQuantizeFuncs_Cartoonish = R"(
vec3 QuantizeLight( in vec3 light )
{
	return floor( light * quantization + vec3(0.5) ) / quantization;
}

float QuantizeDot( in float normalDotGeomToLight )
{
	return floor( 2.0 * normalDotGeomToLight );
}

vec3 QuantizeDiffuse( in vec3 light, in float normalDotGeomToLight )
{
	vec3	result;
	result = mix( QuantizeLight( light * normalDotGeomToLight ), light, 0.3 );
	return result;
}

)";


#pragma mark kDirectionalLightFragmentShaderSource
// This shader code computes the contribution of one directional
// light.  Copies of this code, with LIGHT_INDEX replaced by a
// specific integer, will be included inline.
// input: vec3 normal
// inout: vec3 diff, vec3 spec
const char* kDirectionalLightFragmentShaderSource = R"(
// Directional light, light LIGHT_INDEX
{
	float nDotVP = max( 0.0, dot( normal,
		lightPosition[LIGHT_INDEX].xyz ) );
	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb, nDotVP );

	vec3 geomToLight = normalize(lightPosition[LIGHT_INDEX].xyz);
	vec3 halfVector = normalize( geomToLight + geomToEyeDir );

	float nDotHV = max( 0.0,
		dot( normal, halfVector ) );
	float pf1 = (specularExp <= 0.0)? 1.0 : pow( nDotHV, specularExp );
	float pf = (nDotVP > 0.0)? pf1 : 0.0;
	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf);
}

)";


#pragma mark kDirectionalLightWithNondirIllumFragmentShaderSource
const char* kDirectionalLightWithNondirIllumFragmentShaderSource = R"(
// Directional light, light LIGHT_INDEX
{
	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb, 1.0 );
}

)";


#pragma mark kSpotFalloffNoneSource
const char* kSpotFalloffNoneSource = R"(
// Spot light falloff function, none, for light LIGHT_INDEX
float SpotFalloff_LIGHT_INDEX( in float x )
{
	return 1.0;
}

)";


#pragma mark kSpotFalloffLinearSource
const char* kSpotFalloffLinearSource = R"(
// Spot light falloff function, linear, for light LIGHT_INDEX
float SpotFalloff_LIGHT_INDEX( in float x )
{
	return 1.0 - x;
}

)";


#pragma mark kSpotFalloffExponentialSource
const char* kSpotFalloffExponentialSource = R"(
// Spot light falloff function, exponential, for light LIGHT_INDEX
float SpotFalloff_LIGHT_INDEX( in float x )
{
	return (pow( 10.0, 1.0 - x ) - 1.0) / 9.0;
}

)";


#pragma mark kSpotFalloffCosineSource
const char* kSpotFalloffCosineSource = R"(
// Spot light falloff function, cosine, for light LIGHT_INDEX
float SpotFalloff_LIGHT_INDEX( in float x )
{
	return cos( radians( 90.0 * x ) );
}

)";


#pragma mark kSpotFalloffSmoothCubicSource
const char* kSpotFalloffSmoothCubicSource = R"(
// Spot light falloff function, smooth cubic, for light LIGHT_INDEX
float SpotFalloff_LIGHT_INDEX( in float x )
{
	return 1.0 - smoothstep( 0.0, 1.0, x );
}

)";


#pragma mark kPointLightFragmentShaderSource
// This shader code computes the contribution of one point
// light.  Copies of this code, with LIGHT_INDEX replaced by a
// specific integer, will be included inline.
// input: vec3 geomToEyeDir, vec3 normal
// inout: vec3 diff, vec3 spec
const char* kPointLightFragmentShaderSource = R"(
// Positional light, light LIGHT_INDEX
{
	// Compute vector from surface to light position
	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - FSIN.ECPos4.xyz;

	// Compute distance between geometry and light
	float d = length(geomToLight);

	// Normalize direction from geometry to light
	geomToLight /= d;

	// Compute attenuation factor
	float attenuation = 1.0 /
		(lightAttenuation[LIGHT_INDEX].x +
		lightAttenuation[LIGHT_INDEX].y * d +
		lightAttenuation[LIGHT_INDEX].z * d * d );

	// Compute the direction halfway between the geometry to light vector
	// and the geometry to eye vectors.  This uses the assumption that
	// geomToEyeDir and geomToLight are both normalized.
	vec3 halfVector = normalize( geomToLight + geomToEyeDir );

	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );
	nDotGeomToLight = QuantizeDot( nDotGeomToLight );

	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb *
				attenuation, nDotGeomToLight );

	float nDotHalf = max( 0.0, dot( normal, halfVector ) );

	float pf;
	if (nDotGeomToLight == 0.0)
		pf = 0.0;
	else
		pf = (specularExp <= 0.0)? 1.0 : pow( nDotHalf, specularExp );

	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf * attenuation);
}

)";


#pragma mark kPointLightWithNondirIllumFragmentShaderSource
const char* kPointLightWithNondirIllumFragmentShaderSource = R"(
// Positional light, light LIGHT_INDEX
{
	// Compute vector from surface to light position
	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - FSIN.ECPos4.xyz;

	// Compute distance between geometry and light
	float d = length(geomToLight);

	// Compute attenuation factor
	float attenuation = 1.0 /
		(lightAttenuation[LIGHT_INDEX].x +
		lightAttenuation[LIGHT_INDEX].y * d +
		lightAttenuation[LIGHT_INDEX].z * d * d );

	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb *
				attenuation, 1.0 );
}

)";


#pragma mark kSpotLightFragmentShaderSource
// This shader code computes the contribution of one spot
// light.  Copies of this code, with LIGHT_INDEX replaced by a
// specific integer, will be included inline.
// input: vec3 geomToEyeDir, vec3 normal
// inout: vec3 diff, vec3 spec
const char* kSpotLightFragmentShaderSource = R"(
// Positional light, light LIGHT_INDEX
{
	// Compute vector from surface to light position
	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - FSIN.ECPos4.xyz;

	// Compute distance between geometry and light
	float d = length(geomToLight);

	// Normalize direction from geometry to light
	geomToLight /= d;

	// Compute attenuation factor
	float attenuation = 1.0 /
		(lightAttenuation[LIGHT_INDEX].x +
		lightAttenuation[LIGHT_INDEX].y * d +
		lightAttenuation[LIGHT_INDEX].z * d * d );

	// Find the angle between the vector from the light to this
	// fragment and the light direction vector.  The vectors are
	// unit vectors, so the dot product is the cosine of the angle.
	// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
	// default value of 0, and we need not look at
	// gl_LightSource[LIGHT_INDEX].spotExponent.
	float spotDot = dot( -geomToLight, spotLightDirection[LIGHT_INDEX] );
	float spotAngle = acos( spotDot );

	// Compute a falloff factor.
	float fallFrac = (spotAngle - hotAngle[LIGHT_INDEX]) /
		(cutoffAngle[LIGHT_INDEX] - hotAngle[LIGHT_INDEX]);
	float spotFalloff = (spotAngle < hotAngle[LIGHT_INDEX])?
		1.0 :
		((spotAngle > cutoffAngle[LIGHT_INDEX])?
			0.0 :
			SpotFalloff_LIGHT_INDEX( fallFrac ));
	
	// See if point on surface is inside cone of illumination,
	// and maybe attenuate by angle from center of spot.
	// Set attenuation to 0 if outside the spot light cone.
	attenuation *= spotFalloff;

	// Compute the direction halfway between the geometry to light vector
	// and the geometry to eye vectors.  This uses the assumption that
	// geomToEyeDir and geomToLight are both normalized.
	vec3 halfVector = normalize( geomToLight + geomToEyeDir );

	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );
	nDotGeomToLight = QuantizeDot( nDotGeomToLight );

	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb *
				attenuation, nDotGeomToLight );

	float nDotHalf = max( 0.0, dot( normal, halfVector ) );

	float pf;
	if (nDotGeomToLight == 0.0)
		pf = 0.0;
	else
		pf = (specularExp <= 0.0)? 1.0 : pow( nDotHalf, specularExp );

	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf * attenuation);
}

)";


#pragma mark kSpotLightWithNondirIllumFragmentShaderSource
const char* kSpotLightWithNondirIllumFragmentShaderSource = R"(
// Positional light, light LIGHT_INDEX
{
	// Compute vector from surface to light position
	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - FSIN.ECPos4.xyz;

	// Compute distance between geometry and light
	float d = length(geomToLight);

	// Normalize direction from geometry to light
	geomToLight /= d;

	// Compute attenuation factor
	float attenuation = 1.0 /
		(lightAttenuation[LIGHT_INDEX].x +
		lightAttenuation[LIGHT_INDEX].y * d +
		lightAttenuation[LIGHT_INDEX].z * d * d );

	// Find the angle between the vector from the light to this
	// fragment and the light direction vector.  The vectors are
	// unit vectors, so the dot product is the cosine of the angle.
	// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
	// default value of 0, and we need not look at
	// gl_LightSource[LIGHT_INDEX].spotExponent.
	float spotDot = dot( -geomToLight, spotLightDirection[LIGHT_INDEX] );
	float spotAngle = acos( spotDot );

	// Compute a falloff factor.
	float fallFrac = (spotAngle - hotAngle[LIGHT_INDEX]) /
		(cutoffAngle[LIGHT_INDEX] - hotAngle[LIGHT_INDEX]);
	float spotFalloff = (spotAngle < hotAngle[LIGHT_INDEX])?
		1.0 :
		((spotAngle > cutoffAngle[LIGHT_INDEX])?
			0.0 :
			SpotFalloff_LIGHT_INDEX( fallFrac ));
	
	// See if point on surface is inside cone of illumination,
	// and maybe attenuate by angle from center of spot.
	// Set attenuation to 0 if outside the spot light cone.
	attenuation *= spotFalloff;

	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb *
				attenuation, 1.0 );
}

)";


#pragma mark kMainFragmentShaderStart
const char* kMainFragmentShaderStart = R"(
void main()
{
	// Color components of light, lights will add to these.
	vec3		diff = vec3(0.0);
	vec3		spec = vec3(0.0);

	// Color of fragment, to be determined later.
	vec3	color;
	float	alpha;

	// Specular exponent
	float specularExp = isUsingSpecularMap?
		texture( tex1, FSIN.texCoord ).a * 128.0 :
		shininess;

	// Geometry position and direction to eye
	vec3		geomPos = FSIN.ECPos4.xyz;
	vec3		geomToEyeDir = - normalize( geomPos );

	// Front face normal, used for backface flipping, flat shading
	vec3	faceNormal = cross(dFdx(FSIN.ECPos4.xyz), dFdy(FSIN.ECPos4.xyz));

)";


#pragma mark kFragmentClipping
const char* kFragmentClipping = R"(
	if (dot( vec4( FSIN.ECPos4.xyz, 1.0 ), ECClipPlane ) < 0.0)
	{
		discard;
	}

)";


#pragma mark kMainFragmentShaderStartSmooth
const char* kMainFragmentShaderStartSmooth = R"(
	// Eye coordinate normal vector.  Even if the vertex normals were normalized
	// and the modelview matrix has no scaling, we would still have to normalize
	// here due to interpolation.
	vec3		normal = normalize(FSIN.ECNormal);

	// Flip the normal for the back face.  If we are using
	// backfacing style Remove, then back face triangles will
	// not get here, in which case no harm is done.
	// We have tried several methods, each with disadvantages.
	// 1. Obvious way:
	//		normal = gl_FrontFacing? normal : -normal;
	//	  Problem:  gl_FrontFacing is poorly supported on
	//	  some cards/drivers... it may simply fail, or it may cause
	//    a fallback to software rendering.
	// 2. Vertex normal way:
	//		normal = faceforward( normal, geomPos, normal );
	//    Sometimes, especially in models created by skeletal
	//    animation, the vertex normals may
	//    point a little away from the camera even though the
	//    triangle faces front.
	// 3. 2-sided lighting way:
	//		glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );
	//    and in the vertex shader set gl_FrontSecondaryColor
	//    and gl_BackSecondaryColor differently, and in the
	//    fragment shader say
	//      normal = (gl_SecondaryColor.r > 0.0)? normal : -normal;
	//    Problem: on some ATI cards you may get a failure to
	//    compile shaders, intermittently.
	// 4. Calculus way:  Determine the front face normal as we
	//    had previously done for flat shading, and then say
	//      normal = faceforward( normal, normal, -faceNormal );
	//    That is, we flip the normal if its dot with faceNormal
	//    is negative.
	//    Problems: This can occasionally produce incorrect
	//    results in weird cases where vertex normals are
	//    somewhat toward the back of the triangle.
	//    It was also giving incorrect rendering in a case of using
	//    edge-fill style and backfacing-both style, which I do
	//    not understand.
	normal = isFlippingNormals? (gl_FrontFacing? normal : -normal) : normal;

)";


#pragma mark kMainFragmentShaderStartFlat
/*
	Typically one would do flat shading by using the "flat" interpolation
	qualifier on a normal vector input.  But you can't use such a qualifier
	on just one member of a struct.
 */
const char* kMainFragmentShaderStartFlat = R"(
	// Face normal vector for flat shading.
	vec3	normal = normalize(faceNormal);

	// In this case there is no need to flip for the backface,
	// because faceNormal always is toward the eye.

)";


#pragma mark kColorCompForNULLIllumination
const char* kColorCompForNULLIllumination = R"(
	color = FSIN.interpolatedColor.rgb + quesaEmissiveColor;
	alpha = FSIN.interpolatedColor.a;

)";


#pragma mark kColorCompForLambertAndPhong
const char* kColorCompForLambertAndPhong = R"(
	// Start with emissive and global ambient color.
	// I will assume that the only ambient light is global.
	color = ambientLight * FSIN.interpolatedColor.rgb +
         quesaEmissiveColor;

	// Add diffuse color.
	color += diff * FSIN.interpolatedColor.rgb;

	alpha = FSIN.interpolatedColor.a;

)";


#pragma mark kColorCompForLambertAndPhong_Cartoonish
const char* kColorCompForLambertAndPhong_Cartoonish = R"(
	// Start with emissive and global ambient color.
	// I will assume that the only ambient light is global.
	color = ambientLight * FSIN.interpolatedColor.rgb +
			quesaEmissiveColor;

	// Add diffuse color.
	// Ordinarily we just add the diffuse light times the
	// material color, but when doing a cartoonish style, we
	// may darken areas nearly perpendicular to the eye.
	if (dot( normal, geomToEyeDir ) > 0.5)
	{
		color += min( diff, 1.0 ) * FSIN.interpolatedColor.rgb;
	}
	else
	{
		color += lightNearEdge * min( diff, 1.0 ) * FSIN.interpolatedColor.rgb;
	}

	alpha = FSIN.interpolatedColor.a;

)";


#pragma mark kTexturedColorComp
const char* kTexturedColorComp = R"(
	// Texturing, GL_MODULATE mode
	{
		vec4 texColor = texture( tex0, FSIN.texCoord );
		color *= texColor.rgb;
		alpha *= texColor.a;
		if (texColor.a < alphaThreshold)
		{
			discard;
		}
	}

)";


#pragma mark kAddSpecularColor
const char* kAddSpecularColor = R"(
	// Add specular color.  This is done after texturing, as with
	// separate specular color.
	vec3 specMat = isUsingSpecularMap?
		texture( tex1, FSIN.texCoord ).rgb :
		specularColor;
	color += spec * specMat;

)";


#pragma mark kCalcFogLinear
const char* kCalcFogLinear = R"(
	float dist = length( FSIN.ECPos4.xyz );
	float fog = (linearFogEnd - dist) * linearFogScale;
)";


#pragma mark kCalcFogExp
const char* kCalcFogExp = R"(
	float dist = length( FSIN.ECPos4.xyz );
	float fog = exp( - fogDensity * dist );
)";


#pragma mark kCalcFogExp2
const char* kCalcFogExp2 = R"(
	float dist = length( FSIN.ECPos4.xyz );
	float fogProd = fogDensity * dist;
	float fog = exp( - fogProd * fogProd );
)";


#pragma mark kCalcFogHalfspace
const char* kCalcFogHalfspace = R"(
	vec4 ECPos = vec4(FSIN.ECPos4.xyz, 1.0);
	float p = dot( HSFogPlane, ECPos );
	float c = HSFogPlane.w;
	float D = fogDensity;
	float s = HSFogRate;
	float cpDiff = c - p;
	float adjDiff = sign(cpDiff) * (abs(cpDiff) + 0.000001);
	float outerFactor = length(FSIN.ECPos4.xyz) * D / adjDiff;
	float densOverRate = D / s;
	float rateOverDens = 1.0 / densOverRate;
	float c2 = min( c, 0.0 );
	float p2 = min( p, 0.0 );
	float g = outerFactor * (c2 - p2 - densOverRate *
		(exp( c2 * rateOverDens ) - exp( p2 * rateOverDens )));
	float fog = exp( - g );
)";


#pragma mark kMixFog
/*
	Fog works by mixing the fog color with the fragment color.
	This does not work well with premultiplied alpha, where
	resultColor = 1 * premultipliedFragColor + (1 - alpha) * oldColor,
	because if fog brightens the premultipliedFragColor, then the result
	color will be bright no matter how small alpha is.  Therefore we must
	do the fog mixing with unpremultiplied color.
	
	The FogSmooth function is intended to make the place where you hit the
	maxFogOpacity be a less obvious transition.  It was chosen as a cubic
	polynomial satisfying:
	FogSmooth( 0 ) = 0
	FogSmooth( maxFogOpacity ) = maxFogOpacity
	FogSmooth'( 0 ) = 1				(where the prime indicates derivative)
	FogSmooth'( maxFogOpacity ) = 0
*/
const char* kMixFog = R"(
	float opacity = 1.0 - fog;
	opacity = (maxFogOpacity > 0.99)?
		clamp( opacity, 0.0, 1.0) :
		((opacity < maxFogOpacity)?
			 FogSmooth(opacity) :
			maxFogOpacity);
	float unAlpha = 1.0 / (alpha + 0.0000001);
	vec3 unPreColor = clamp( unAlpha * color, 0.0, 1.0 );
	unPreColor = mix( unPreColor, fogColor, opacity );
	color = alpha * unPreColor;
)";


#pragma mark kAngleAffectOnAlpha
/*
	If you have a sheet of translucent material at an angle to the eye, then you are
	looking through more of it, so it should block more of the light.  The way the
	math works out, the effective alpha is 1 - (1 - alpha)^(1/cos( angle )) .
	The z coordinate of the eye normal vector is the cosine of the angle between the
	fragment normal vector and the view vector.
	When we change the alpha, we must un-premultiply and re-premultiply the color.
*/
const char*	kAngleAffectOnAlpha = R"(
	if ( (alpha < 0.999) && (normal.z > 0.0001) )
	{
		float unAlpha = 1.0 / (alpha + 0.0000001);
		vec3 unPreColor = clamp( unAlpha * color, 0.0, 1.0 );
		alpha = 1.0 - pow( 1.0 - alpha, 1.0/normal.z );
		color = alpha * unPreColor;
	}
)";


#pragma mark kMainFragmentShaderEndSource
const char* kMainFragmentShaderEndSource = R"(
 	fragColor.rgb = color;
	fragColor.a = alpha;
}
)";
}
