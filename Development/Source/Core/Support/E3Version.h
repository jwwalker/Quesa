/*  NAME:
        E3Version.h

    DESCRIPTION:
        Build version header for Quesa.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef E3VERSION_HDR
#define E3VERSION_HDR
//=============================================================================
//      Release macros
//-----------------------------------------------------------------------------
#define E3_FOUR_BYTE_CONSTANT(a,b,c,d)              \
            ((const TQ3Uns32)                       \
            ((const TQ3Uns32) (a) << 24) |          \
            ((const TQ3Uns32) (b) << 16) |          \
            ((const TQ3Uns32) (c) << 8)  |          \
            ((const TQ3Uns32) (d)))





//=============================================================================
//      Release constants
//-----------------------------------------------------------------------------
#define kDevelopment                0x20            // Development release
#define kAlpha                      0x40            // Alpha release
#define kBeta                       0x60            // Beta release
#define kFinal                      0x80            // Final release





//=============================================================================
//      Version information
//-----------------------------------------------------------------------------
//      Note :  The same version information is represented in three ways, and
//              you should change all ways consistently whenever you change the
//              version info:
//                  1) kQ3MajorVersion ... kQ3PrereleaseVersion (4 separate bytes)
//                  2) kQ3StringVersion (string)
//                  3) kQ3PackedVersion (4-byte word)
//      Note :  kQ3MajorVersion, kQ3MinorVersion and kQ3PrereleaseVersion are
//              each in binary-code-decimal format. For example, "1.2a56" is
//              represented as 0x01,0x20,0x40,0x56. Note that the hexidecimal
//              digits a..f should not occur in a version number.
//      Note :  While kQ3MajorVersion and kQ3PrereleaseVersion occur BEFORE a
//              decimal place, kQ3MinorVersion occurs AFTER the decimal place.
//              Thus ".2" (= ".20" = ".2.0") is represented as 0x20, NOT 0x02.
//      For more information, see the description of the 'vers' resource in
//          "Inside Macintosh: Macintosh Toolbox Essential", p. 7-69.
//-----------------------------------------------------------------------------
#define kQ3MajorVersion             0x01            // Major version for build
#define kQ3MinorVersion             0x60            // Minor version for build
#define kQ3ReleaseType              kDevelopment    // Release type for build
#define kQ3PrereleaseVersion        0x14            // Prerelease version for build
#define kQ3StringVersion            "1.6d14"        // Stringified version of above
#define kQ3PackedVersion            E3_FOUR_BYTE_CONSTANT(kQ3MajorVersion,  \
                                                          kQ3MinorVersion,  \
                                                          kQ3ReleaseType,   \
                                                          kQ3PrereleaseVersion)
                                    

#endif
