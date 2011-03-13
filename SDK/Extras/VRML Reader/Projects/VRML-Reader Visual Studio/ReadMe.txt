This project requires the Spirit parser library, which is part of the Boost
libraries available from www.boost.org.  You do not need to install any
executable binaries, just the headers.

After installing the Boost files on your computer, Open the Property Manager
window from the View menu in Visual Studio.  You may need to first switch to
Expert mode.  Then double-click the property sheet named Microsoft.Cpp.Win32.user.
This will open a property pages dialog.  Select "User Macros" in the list on
the left.  Click the "Add Macro" button.  Define a macro named BoostDir whose
value is the path to the Boost directory on your computer.  This should not be
the directory named "boost", but its parent.  For example, on my computer, the
path is currently C:\Code\boost_1_46_0.