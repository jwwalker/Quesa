/*  NAME:
        E3FFW_3DMFBin_Writer.h

    DESCRIPTION:
        Header file for E3FFW_3DMFBin_Writer.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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

