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

## Theory of Operation

Associated with each Quesa view object (type `TQ3View`), there should be a draw context
(representing the more platform-dependent aspects of the viewing destination), a camera,
and a renderer.  When you want to render some Quesa content, you run a *renderering loop*
that looks something like this:

```c++
if (kQ3Success == Q3View_StartRendering( viewObject ))
{
    do
    {
        Q3Object_Submit( object1, viewObject );
        Q3Object_Submit( object2, viewObject );
        ...
    } while (kQ3ViewStatusRetraverse == Q3View_EndRendering( viewObject ));
}
```

Bear in mind that submitting a group may cause a complex hierarchy of geometries,
transforms, and style objects to be sent to the renderer.

Quesa was designed with the intent that one could write plug-in renderers.  Depending on
the platform and back-end technology you are targeting, it might also be necessary to
write a new draw context class.  But for this sample program, we have chosen a more ad-hoc
approach.

In order to avoid any platform-specific code, we use a generic draw context (created using
`Q3GenericDrawContext_New`).  One may be tempted to also use a generic renderer (created
using `Q3Renderer_NewFromType( kQ3RendererTypeGeneric )`, but there's a problem:  The
generic renderer "claims" to be able to render all kinds of geometries, including high
level geometries such as Torus and Cone, whereas Filament only supports lower level types
of geometry such as triangle meshes and lines.  Therefore we create a subclass of the
generic renderer that is unable to render anything, which will cause high-level geometries
to be decomposed into lower-level geometry during rendering.  One more trick is to
subclass the view object so that we can intercept the submit-for-rendering method.

Whenever our submit-for-rendering method sees a geometry of an appropriate type such as
TriMesh or Line, we convert it to Filament geometry data, or look it up in a cache.  It
is appropriate to maintain a cache, because a Quesa hierarchy might reference a geometry
multiple times, with different positions or styles.  We then create a Filament renderable
referencing that geometry data.  We get the current transform from the view using
`Q3View_GetLocalToWorldMatrixState` and give the renderable the corresponding transform.
And we set the material of the renderable using the attribute set of the Quesa geometry
and various style states retrieved from the view.  We don't need to worry about the
structure of whatever Quesa group hierarchy is being submitted, because Quesa keeps track
of the current transform and styles.
