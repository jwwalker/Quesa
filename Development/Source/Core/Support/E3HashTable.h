/*  NAME:
        E3hashTable.h

    DESCRIPTION:
        Header file for E3hashTable.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef E3HASHTABLE_HDR
#define E3HASHTABLE_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Hash tables are opaque
typedef struct E3HashTable *E3HashTablePtr;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Create/destroy a hash table
E3HashTablePtr		E3HashTable_Create(TQ3Uns32 tableSize);
void				E3HashTable_Destroy(E3HashTablePtr theTable);


// Add/remove an item to/from a hash table
TQ3Status			E3HashTable_Add(E3HashTablePtr    theTable, TQ3ObjectType theKey, void *theItem);
void				E3HashTable_Remove(E3HashTablePtr theTable, TQ3ObjectType theKey);


// Find an item in a hash table
void				*E3HashTable_Find(E3HashTablePtr theTable, TQ3ObjectType theKey);


// Get info on a hash table
TQ3Uns32			E3HashTable_GetCollisionMax(E3HashTablePtr theTable);
float				E3HashTable_GetCollisionAverage(E3HashTablePtr theTable);
TQ3Uns32			E3HashTable_GetNumItems(E3HashTablePtr theTable);
TQ3Uns32			E3HashTable_GetTableSize(E3HashTablePtr theTable);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

