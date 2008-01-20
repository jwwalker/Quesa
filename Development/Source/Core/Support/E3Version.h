/*  NAME:
        E3Version.h

    DESCRIPTION:
        Build version header for Quesa.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
#ifndef E3VERSION_HDR
#define E3VERSION_HDR
//=============================================================================
//      Release macros
//-----------------------------------------------------------------------------
#ifndef PLIST_COMPILER
#define E3_FOUR_BYTE_CONSTANT(a,b,c,d)              \
            ((const TQ3Uns32)                       \
            ((const TQ3Uns32) (a) << 24) |          \
            ((const TQ3Uns32) (b) << 16) |          \
            ((const TQ3Uns32) (c) << 8)  |          \
            ((const TQ3Uns32) (d)))
#endif




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
//      Note :  kQ3MajorVersion and kQ3MinorVersion are in binary-code-decimal
//              format, whereas kQ3PrereleaseVersion is simply an unsigned
//				integer. For example, "1.2.3a46" is represented as
//              0x01,0x23,0x40,0x2E. Note that the hexadecimal digits a..f can
//              occur only in the kQ3PrereleaseVersion part of in a version number.
//      Note :  While kQ3MajorVersion and kQ3PrereleaseVersion occur BEFORE a
//              decimal place, kQ3MinorVersion occurs AFTER the decimal place.
//              Thus ".2" (= ".20" = ".2.0") is represented as 0x20, NOT 0x02.
//      For more information, see the "Version Territory" tech note,
//			<http://developer.apple.com/technotes/tn/tn1132.html>.
//-----------------------------------------------------------------------------
#define kQ3MajorVersion             0x02            // Major version for build
#define kQ3MinorVersion             0x00            // Minor version for build
#define kQ3ReleaseType              kFinal		    // Release type for build
#define kQ3PrereleaseVersion        0x00            // Prerelease version for build
#define kQ3UnquotedStringVersion    2.0             // Stringified version of above
#define kQ3StringVersion            "2.0"			// quoted string version

#ifndef PLIST_COMPILER
#define kQ3PackedVersion            E3_FOUR_BYTE_CONSTANT(kQ3MajorVersion,  \
                                                          kQ3MinorVersion,  \
                                                          kQ3ReleaseType,   \
                                                          kQ3PrereleaseVersion)
#endif

#endif
