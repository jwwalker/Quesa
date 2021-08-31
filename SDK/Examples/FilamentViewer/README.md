## What is FilamentViewer

FilamentViewer is a sample of how to do rendering, cameras, and lighting using
[Filament](https://github.com/google/filament) while using Quesa as a file format
and scene graph.  Not only Quesa geometries and materials, but also many Quesa style and
transform objects, are converted to Filament information.

Filament  is a multi-platform rendering engine, but this sample is
currently Mac-only.  It can open and display a 3dmf file and let you play with options.

The sample project has two targets, one using Metal as Filament's back end rendering
technology, and one using OpenGL.

FilamentViewer was last tested with Filament version 1.12.2.

## Prerequisites

You will need binaries and includes of Filament, which you can get either by doing builds
of the full Filament source repository or by downloading a release archive from the
[Filament site on GitHub](https://github.com/google/filament).

You also need to build the Framework_NoOpenGL target of the Mac Quesa project.

## Setup

Before attempting to build the project, go into Xcode's Preferences window, select the
**Locations** icon in the tool bar, and select the **Custom Paths** item under that.
Add a path named **Quesa** pointing to the Quesa repository directory, and add a path
named **FilamentRelease** pointing to the Filament release directory.  The latter should
have subdirectories named "bin", "include", and "lib".  After setting the custom paths,
quit and reopen Xcode before building.

The project is currently set up to build for the x86_64 architecture only.  If you want to
build for arm64 (Apple Silicon) as well, you will need to make sure you have universal
builds of the Filament libraries, and change 2 lines of the
`proj.xcconfig` file:

`ARCHS = x86_64 arm64`

`LIBRARY_SEARCH_PATHS = "$FilamentRelease/lib/universal"`
