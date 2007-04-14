#pragma once
/*  NAME:
        quesa-methods.h

    DESCRIPTION:
        Header file for quesa-methods.cp.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    ___________________________________________________________________________
*/
#if __MACH__
	#include <Quesa/Quesa.h>
#else
	#include <Quesa.h>
#endif

/*!
	@function	Register_3DS_Class
	
	@abstract	Register the 3DS Reader-Writter classes with Quesa.
	
	@result		Success or failure of the operation.
*/
TQ3Status		Register_3DS_Class();

/*!
	@function	Unregister_3DS_Class
	
	@abstract	Unregister the 3DS Reader-Writter classes from Quesa.
	
	@discussion	Typically, one will use Quesa and its plugins until the
				process quits, so it will not be important to unregister
				classes.
	
	@result		Success or failure of the operation.
*/
TQ3Status		Unregister_3DS_Class();

