/*  NAME:
        E3FFW_3DMFBin_Writer.h

    DESCRIPTION:
        Header file for E3FFW_3DMFBin_Writer.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
#ifndef E3FFW_3DMFBIN_WRITER_HDR
#define E3FFW_3DMFBIN_WRITER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here

#include "E3FFR_3DMF.h"




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3XView_SubmitWriteData(TQ3ViewObject view, TQ3Size size, void *data, TQ3XDataDeleteMethod deleteData);
TQ3Status			E3XView_SubmitSubObjectData(TQ3ViewObject view, TQ3XObjectClass objectClass, TQ3Uns32 size, void *data, TQ3XDataDeleteMethod deleteData);

TQ3Status			E3FFW_3DMF_StartFile(
						TQ3ViewObject				theView,
						TE3FFormatW3DMF_Data		*fileFormatPrivate,
						TQ3DrawContextObject	theDrawContext);
								
TQ3ViewStatus		E3FFW_3DMF_EndPass(
						TQ3ViewObject				theView,
						TE3FFormatW3DMF_Data		*fileFormatPrivate);
								
void				E3FFW_3DMF_Cancel(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data		*fileFormatPrivate);

TQ3Status			E3FFW_3DMF_Close( TQ3FileFormatObject format, TQ3Boolean abort );

TQ3Status
E3FFW_3DMF_Group(TQ3ViewObject       theView,
						void                *fileFormatPrivate,
						TQ3GroupObject   theGroup,
						TQ3ObjectType		objectType,
						const void		*objectData);

TQ3Status
E3FFW_3DMF_TraverseObject(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data		*fileFormatPrivate,
					TQ3Object		theObject,
					TQ3ObjectType		objectType,
					const void			*objectData);



// Write methods for the 3DMF root object and other support classes

TQ3Status			E3FFW_3DMF_Void_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);
void					E3FFW_3DMF_Default_Delete(void *data);
TQ3Status			E3FFW_3DMF_32_Write(const void *object,  TQ3FileObject theFile);
TQ3Status			E3FFW_3DMF_32_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);

TQ3Status			E3FFW_3DMF_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);
TQ3Status			E3FFW_3DMF_Write(const void *object,  TQ3FileObject theFile);

TQ3Status			E3FFW_3DMF_TOC_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);
TQ3Status			E3FFW_3DMF_TOC_Write(const void *object,  TQ3FileObject theFile);

TQ3Status			E3FFW_3DMF_Reference_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);

TQ3Status			E3FFW_3DMF_type_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);
TQ3Status			E3FFW_3DMF_type_Write(const void *object,  TQ3FileObject theFile);

TQ3Status			E3FFW_3DMF_DisplayGroup_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);

TQ3Status			E3FFW_3DMF_CString_Traverse(TQ3Object object,  void *data,  TQ3ViewObject view);
TQ3Status			E3FFW_3DMF_CString_Write(const void *object,  TQ3FileObject theFile);

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

