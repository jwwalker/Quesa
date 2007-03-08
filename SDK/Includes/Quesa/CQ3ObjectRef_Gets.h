/*!
	@header		CQ3ObjectRef_Gets.h
	
	This header provides C++ wrappers for Quesa functions that return an object
	reference in an output parameter, as a convenience.  In failure conditions,
	the CQ3ObjectRef may contain NULL.  The name of the wrapper is the name of
	the original function with a C prepended.
*/

/*  NAME:
        CQ3ObjectRef_Gets.h

    DESCRIPTION:
        C++ wrappers for Quesa functions that return an object reference in an
		output parameter.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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

#include "Quesa.h"
#include "QuesaGeometry.h"
#include "QuesaGroup.h"
#include "QuesaIO.h"
#include "QuesaShader.h"
#include "QuesaStyle.h"
#include "QuesaView.h"
#include "CQ3ObjectRef.h"

#pragma mark Functions from Quesa.h
/*!
	@functiongroup	Functions from Quesa.h
*/

/*!
 *  @function		CQ3Object_GetSet
 *  @discussion
 *      Get the element set of an object.  This is not the same as the set
 *		returned by <code>Q3Shape_GetSet</code>.
 *
 *      This function is primarily for the use of file format plug-ins.
 *		Most applications will not need it.
 *
 *  @param	inObject    		The object.
 *	@result		A set object, or NULL.
 */
inline CQ3ObjectRef	CQ3Object_GetSet( TQ3Object inObject )
{
	TQ3SetObject	theSet = NULL;
	Q3Object_GetSet( inObject, &theSet );
	return CQ3ObjectRef( theSet );
}

/*!
 *  @function		CQ3Shape_GetSet
 *  @discussion
 *      Get the set element currently associated with a shape.
 *		The function call <code>Q3Shape_GetSet( o, &s )</code> is equivalent to
 *		<code>Q3Shape_GetElement( o, kQ3ElementTypeSet, &s )</code>.
 *
 *      Note that this is not the same as the attribute set of a geometry object,
 *		nor is in the internal set which holds elements on an object.  That is,
 *		after adding elements or attributes to an object, this function may still
 *		return the NULL set.
 *
 *  @param shape            The object to query.
 *  @param theSet           Receives the set of the object, or NULL.
 *  @result                 Success or failure of the operation.
 */
inline CQ3ObjectRef	CQ3Shape_GetSet( TQ3ShapeObject inObject )
{
	TQ3SetObject	theSet = NULL;
	Q3Shape_GetSet( inObject, &theSet );
	return CQ3ObjectRef( theSet );
}


#pragma mark Functions from QuesaGeometry.h
/*!
	@functiongroup	Functions from QuesaGeometry.h
*/


/*!
 *  @function		CQ3Geometry_GetAttributeSet
 *  @discussion
 *      Returns the attribute set associated with a geometric object.
 *
 *  @param geometry         A reference to a geometry object.
 *  @result                 The attribute set of the geometry.
 */
inline CQ3ObjectRef	CQ3Geometry_GetAttributeSet(
	TQ3GeometryObject             geometry )
{
	TQ3AttributeSet	atts = NULL;
	if (kQ3Failure == Q3Geometry_GetAttributeSet( geometry, &atts ))
	{
		atts = NULL;
	}
	return CQ3ObjectRef( atts );
}


/*!
 *  @function		CQ3Box_GetFaceAttributeSet
 *  @discussion
 *      Gets the attribute set associated with a certain face of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box,
 *							between 0 and 5 (inclusive).
 *  @result                 The attribute set of the box face, or
 *							NULL if the face does not have an attribute set.
 */
inline CQ3ObjectRef CQ3Box_GetFaceAttributeSet(
	TQ3GeometryObject             box,
    TQ3Uns32                      faceIndex )
{
	TQ3AttributeSet	atts = NULL;
	Q3Box_GetFaceAttributeSet( box, faceIndex, &atts );
	return CQ3ObjectRef( atts );
}


/*!
 *  @function		CQ3Cone_GetBottomAttributeSet
 *  @discussion		Get the attribute set for the bottom cap of a cone object.
 *
 *  @param cone             	The cone object.
 *  @result                 	The bottom attribute set, or NULL.
 */
inline CQ3ObjectRef CQ3Cone_GetBottomAttributeSet(
	TQ3GeometryObject             cone )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cone_GetBottomAttributeSet( cone, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Cone_GetFaceAttributeSet
 *  @discussion		Get the attribute set for the face (as opposed to the bottom
 *					cap) of a cone.
 *
 *  @param cone             	The cone object.
 *  @result                 	The face attribute set, or NULL.
 */
inline CQ3ObjectRef CQ3Cone_GetFaceAttributeSet(
	TQ3GeometryObject             cone )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cone_GetFaceAttributeSet( cone, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Cone_GetInteriorAttributeSet
 *  @discussion
 *      Get the attribute set for the interior of a cone object.
 *
 *  @param cylinder         A cone object.
 *  @result                 The attribute set for the interior, or NULL.
 */
inline CQ3ObjectRef CQ3Cone_GetInteriorAttributeSet(
	TQ3GeometryObject             cone )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cone_GetInteriorAttributeSet( cone, &atts );
	return CQ3ObjectRef( atts );
}


/*!
 *  @function		CQ3Cylinder_GetTopAttributeSet
 *  @discussion		Get the attribute set for the top cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @result                 The attribute set for the top cap, or NULL.
 */
inline CQ3ObjectRef CQ3Cylinder_GetTopAttributeSet(
	TQ3GeometryObject             cylinder )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cylinder_GetTopAttributeSet( cylinder, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Cylinder_GetBottomAttributeSet
 *  @discussion		Get the attribute set for the bottom cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @result                 The attribute set for the bottom cap, or NULL.
 */
inline CQ3ObjectRef CQ3Cylinder_GetBottomAttributeSet(
	TQ3GeometryObject             cylinder )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cylinder_GetBottomAttributeSet( cylinder, &atts );
	return CQ3ObjectRef( atts );
}


/*!
 *  @function		CQ3Cylinder_GetFaceAttributeSet
 *  @discussion		Get the attribute set for the face of a cylinder object
 *					(i.e., neither end)
 *  @param cylinder         A cylinder object.
 *  @result                 The attribute set for the face, or NULL.
 */
inline CQ3ObjectRef CQ3Cylinder_GetFaceAttributeSet(
	TQ3GeometryObject             cylinder )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cylinder_GetFaceAttributeSet( cylinder, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Cylinder_GetInteriorAttributeSet
 *  @discussion		Get the attribute set for the interior of a cylinder object.
 *  @param cylinder         A cylinder object.
 *  @result                 The attribute set for the interior, or NULL.
 */
inline CQ3ObjectRef CQ3Cylinder_GetInteriorAttributeSet(
	TQ3GeometryObject             cylinder )
{
	TQ3AttributeSet	atts = NULL;
	Q3Cylinder_GetInteriorAttributeSet( cylinder, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3GeneralPolygon_GetVertexAttributeSet
 *  @discussion		Get the attribute set of a vertex of a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @result                 The vertex attribute set, or NULL.
 */
inline CQ3ObjectRef	CQ3GeneralPolygon_GetVertexAttributeSet(
	TQ3GeometryObject             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex )
{
	TQ3AttributeSet	atts = NULL;
	Q3GeneralPolygon_GetVertexAttributeSet( generalPolygon, contourIndex,
		pointIndex, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Line_GetVertexAttributeSet
 *  @discussion		Get the attribute set of one of the ends of a line object.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @result					The attribute set, or NULL.
 */
inline CQ3ObjectRef CQ3Line_GetVertexAttributeSet(
	TQ3GeometryObject             line,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3Line_GetVertexAttributeSet( line, index, &atts );
	return CQ3ObjectRef( atts );
}


/*!
 *  @function		CQ3Mesh_GetVertexAttributeSet
 *  @discussion		Get the attribute set of a mesh vertex.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @result                 The attribute set of the mesh vertex.
 */
inline CQ3ObjectRef CQ3Mesh_GetVertexAttributeSet(
	TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex )
{
	TQ3AttributeSet	atts = NULL;
	Q3Mesh_GetVertexAttributeSet( mesh, vertex, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Mesh_GetFaceAttributeSet
 *  @discussion		Get the attribute set of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @result                 The attribute set of the mesh face.
 */
inline CQ3ObjectRef CQ3Mesh_GetFaceAttributeSet(
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face )
{
	TQ3AttributeSet	atts = NULL;
	Q3Mesh_GetFaceAttributeSet( mesh, face, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Mesh_GetEdgeAttributeSet
 *  @discussion		Get the attribute set of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @result                 The attribute set of the mesh face.
 */
inline CQ3ObjectRef CQ3Mesh_GetEdgeAttributeSet(
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge )
{
	TQ3AttributeSet	atts = NULL;
	Q3Mesh_GetEdgeAttributeSet( mesh, edge, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Mesh_GetCornerAttributeSet
 *  @discussion		Get the attribute set of a mesh corner.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param face             The face to query.
 *  @result                 The attribute set of the mesh corner.
 */
inline CQ3ObjectRef CQ3Mesh_GetCornerAttributeSet(
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
    TQ3MeshFace                   face )
{
	TQ3AttributeSet	atts = NULL;
	Q3Mesh_GetCornerAttributeSet( mesh, vertex, face, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Polygon_GetVertexAttributeSet
 *  @discussion		Get the attribute set of a vertex in a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @result                 The attribute set, or NULL.
 */
inline CQ3ObjectRef CQ3Polygon_GetVertexAttributeSet(
	TQ3GeometryObject             polygon,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3Polygon_GetVertexAttributeSet( polygon, index, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Polyhedron_GetVertexAttributeSet
 *  @discussion		Get the position of a polyhedron vertex.
 *
 *  @param polyhedron       The polyhedron to query.
 *  @param index            The index of the vertex to query.
 *  @result                 The attribute set of the vertex.
 */
inline CQ3ObjectRef CQ3Polyhedron_GetVertexAttributeSet(
	TQ3GeometryObject             polyhedron,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3Polyhedron_GetVertexAttributeSet( polyhedron, index, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3PolyLine_GetVertexAttributeSet
 *  @discussion		Get the attribute set of a polyline vertex.
 *
 *  @param polyLine         The polyline to query.
 *  @param index            The index of the vertex to query.
 *  @result                 The attribute set of the polyline vertex.
 */
inline CQ3ObjectRef CQ3PolyLine_GetVertexAttributeSet(
	TQ3GeometryObject             polyLine,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3PolyLine_GetVertexAttributeSet( polyLine, index, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3PolyLine_GetSegmentAttributeSet
 *  @discussion		Get the attribute set of a polyline segment.
 *
 *  @param polyLine         The polyline to query.
 *  @param index            The index of the segment to query.
 *  @result                 The attribute set of the polyline segment.
 */
inline CQ3ObjectRef CQ3PolyLine_GetSegmentAttributeSet(
	TQ3GeometryObject             polyLine,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3PolyLine_GetSegmentAttributeSet( polyLine, index, &atts );
	return CQ3ObjectRef( atts );
}

/*!
 *  @function		CQ3Triangle_GetVertexAttributeSet
 *  @discussion		Get the attribute set of a triangle vertex.
 *
 *  @param triangle         The triangle to query.
 *  @param index            The index of the vertex to query.
 *  @result                 The attribute set of the triangle vertex.
 */
inline CQ3ObjectRef CQ3Triangle_GetVertexAttributeSet(
	TQ3GeometryObject             triangle,
    TQ3Uns32                      index )
{
	TQ3AttributeSet	atts = NULL;
	Q3Triangle_GetVertexAttributeSet( triangle, index, &atts );
	return CQ3ObjectRef( atts );
}

#pragma mark Functions from QuesaGroup.h
/*!
	@functiongroup	Functions from QuesaGroup.h
*/

/*!
 *  @function		CQ3Group_GetPositionObject
 *  @discussion
 *      Get the object located at a certain position in a group.
 *
 *		This function obtains a reference to the group member at a 
 *		specified position.
 *
 *      Returns NULL if there is no object at the given position.
 *
 *  @param group            Group to inspect.
 *  @param position         Position of desired object.
 *  @result                 The indicated object.
 */
inline CQ3ObjectRef CQ3Group_GetPositionObject(
	TQ3GroupObject                group,
    TQ3GroupPosition              position )
{
	TQ3Object	theObject = NULL;
	Q3Group_GetPositionObject( group, position, &theObject );
	return CQ3ObjectRef( theObject );
}

#pragma mark Functions from QuesaIO.h
/*!
	@functiongroup	Functions from QuesaIO.h
*/

/*!
 *  @function		CQ3File_GetStorage
 *  @discussion		Get the storage object for a file.
 *
 *  @param theFile          The file to query.
 *  @result                 The storage object for the file.
 */
inline CQ3ObjectRef CQ3File_GetStorage(
	TQ3FileObject                 theFile )
{
	TQ3StorageObject	theStorage = NULL;
	Q3File_GetStorage( theFile, &theStorage );
	return CQ3ObjectRef( theStorage );
}

/*!
 *  @function		CQ3ViewHints_GetRenderer
 *  @discussion		Get the renderer from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @result                 The renderer from the view hints.
 */
inline CQ3ObjectRef CQ3ViewHints_GetRenderer(
	TQ3ViewHintsObject            viewHints )
{
	TQ3Object	theObject = NULL;
	Q3ViewHints_GetRenderer( viewHints, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3ViewHints_GetCamera
 *  @discussion		Get the camera from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @result                 The camera from the view hints.
 */
inline CQ3ObjectRef CQ3ViewHints_GetCamera(
	TQ3ViewHintsObject            viewHints )
{
	TQ3Object	theObject = NULL;
	Q3ViewHints_GetCamera( viewHints, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3ViewHints_GetLightGroup
 *  @discussion		Get the light group from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @result                 The light group from the view hints.
 */
inline CQ3ObjectRef CQ3ViewHints_GetLightGroup(
	TQ3ViewHintsObject            viewHints )
{
	TQ3Object	theObject = NULL;
	Q3ViewHints_GetLightGroup( viewHints, &theObject );
	return CQ3ObjectRef( theObject );
}


/*!
 *  @function		CQ3ViewHints_GetAttributeSet
 *  @discussion		Get the attribute set from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @result                 The attribute set from the view hints.
 */
inline CQ3ObjectRef CQ3ViewHints_GetAttributeSet(
	TQ3ViewHintsObject            viewHints )
{
	TQ3Object	theObject = NULL;
	Q3ViewHints_GetAttributeSet( viewHints, &theObject );
	return CQ3ObjectRef( theObject );
}

#pragma mark Functions from QuesaShader.h
/*!
	@functiongroup	Functions from QuesaShader.h
*/

/*!
 *  @function		CQ3TextureShader_GetTexture
 *  @discussion		Get the texture from a texture shader.
 *
 *  @param shader           The shader to query.
 *  @result                 The texture from the texture shader.
 */
inline CQ3ObjectRef CQ3TextureShader_GetTexture(
	TQ3ShaderObject               shader )
{
	TQ3Object	theObject = NULL;
	Q3TextureShader_GetTexture( shader, &theObject );
	return CQ3ObjectRef( theObject );
}

#pragma mark Functions from QuesaStyle.h
/*!
	@functiongroup	Functions from QuesaStyle.h
*/


/*!
 *  @function		CQ3HighlightStyle_Get
 *  @discussion		Get the data from a highlight style.
 *
 *  @param highlight             The style to query.
 *  @result                      The data from the style.
 */
inline CQ3ObjectRef CQ3HighlightStyle_Get(
	TQ3StyleObject                highlight )
{
	TQ3Object	theObject = NULL;
	Q3HighlightStyle_Get( highlight, &theObject );
	return CQ3ObjectRef( theObject );
}

#pragma mark Functions from QuesaView.h
/*!
	@functiongroup	Functions from QuesaView.h
*/


/*!
 *  @function		CQ3View_GetRenderer
 *  @discussion		Get the renderer from a view.
 *
 *  @param view             The view to query.
 *  @result                 The renderer associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetRenderer(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetRenderer( view, &theObject );
	return CQ3ObjectRef( theObject );
}


/*!
 *  @function		CQ3View_GetCamera
 *  @discussion		Get the camera from a view.
 *
 *  @param view             The view to query.
 *  @result                 The camera associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetCamera(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetCamera( view, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3View_GetLightGroup
 *  @discussion		Get the light group from a view.
 *
 *  @param view             The view to query.
 *  @result                 The light group associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetLightGroup(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetLightGroup( view, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3View_GetDrawContext
 *  @discussion		Get the draw context from a view.
 *
 *  @param view             The view to query.
 *  @result                 The draw context associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetDrawContext(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetDrawContext( view, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3View_GetDefaultAttributeSet
 *  @discussion		Get the default attribute set from a view.
 *
 *  @param view             The view to query.
 *  @result                 The default attribute set associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetDefaultAttributeSet(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetDefaultAttributeSet( view, &theObject );
	return CQ3ObjectRef( theObject );
}

/*!
 *  @function		CQ3View_GetAttributeSetState
 *  @discussion		Get the current attribute state set from a view.
 *
 *  @param view             The view to query.
 *  @result                 The current attribute state set associated with the view.
 */
inline CQ3ObjectRef CQ3View_GetAttributeSetState(
	TQ3ViewObject                 view )
{
	TQ3Object	theObject = NULL;
	Q3View_GetAttributeSetState( view, &theObject );
	return CQ3ObjectRef( theObject );
}

// Work around a HeaderDoc bug
/*!
	@functiongroup
*/
