/*  NAME:
        QutUnix.c

    DESCRIPTION:
        Quesa Utility Toolkit - Unix (X11 through GTK).

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"
#include "QutInternal.h"

#include <GL/glx.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
GtkWidget		*gMenuBar         = NULL;
GtkWidget		*gWindowContent   = NULL;
TQ3Uns32		gSpecialItemCount = 0;
int				gOpenGLAttributes[] = { GLX_RGBA,
										GLX_RED_SIZE,   1,
										GLX_GREEN_SIZE, 1,
										GLX_BLUE_SIZE,  1,
										GLX_DEPTH_SIZE, 1,
										GLX_DOUBLEBUFFER,
										None };





//=============================================================================
//		Internal functions.
//-----------------------------------------------------------------------------
//      qut_allocation_to_area : Convert a GtkAllocation to a TQ3Area.
//-----------------------------------------------------------------------------
static void
qut_allocation_to_area(GtkAllocation *theAllocation, TQ3Area *theArea)
{


	// Convert the size
	theArea->min.x = (float) theAllocation->x;
	theArea->min.y = (float) theAllocation->y;
	theArea->max.x = (float) (theAllocation->x + theAllocation->width);;
	theArea->max.y = (float) (theAllocation->y + theAllocation->height);
}





//=============================================================================
//		gdk_gl_choose_visual : Choose a suitable visual.
//-----------------------------------------------------------------------------
static GdkVisual
*gdk_gl_choose_visual(void)
{	GdkVisual		*visual;
	Display			*dpy;
	XVisualInfo		*vi;


	dpy = GDK_DISPLAY();
	if ((vi = glXChooseVisual(dpy,DefaultScreen(dpy), gOpenGLAttributes)) == NULL)
		return(NULL);

	visual = gdkx_visual_get(vi->visualid);
	XFree(vi);

	return visual;
}





//=============================================================================
//		qut_handle_menu_about : Display the about box.
//-----------------------------------------------------------------------------
static void
qut_handle_menu_about(GtkWidget *theWidget, gpointer theData)
{	GtkWidget	*theDialog, *theLabel, *buttonOK;



	// Create the widgets
	theDialog = gtk_dialog_new();
	theLabel  = gtk_label_new("\n"
							  "   Qut App Framework   \n"
							  "   Part of the Quesa Distribution   \n"
							  "\n"
							  "   For more information on Quesa, please   \n"
							  "   see the Quesa home page:   \n"
							  "\n"
							  "   http://www.quesa.org/   \n");
	buttonOK  = gtk_button_new_with_label("OK");



	// Make the OK button close the dialog
	gtk_signal_connect_object(GTK_OBJECT(buttonOK), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(theDialog));
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(theDialog)->action_area), buttonOK);



	// Add the text, and show the dialog
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(theDialog)->vbox), theLabel);
	gtk_widget_show_all(theDialog);
}





//=============================================================================
//		qut_handle_menu_renderer : Handle the renderer menu.
//-----------------------------------------------------------------------------
static void
qut_handle_menu_renderer(GtkWidget *theWidget, gpointer theData)
{	TQ3Uns32		menuItem;



	// Select the renderer
	menuItem = (TQ3Uns32) theData;

	Q3View_SetRendererByType(gView, gRenderers[menuItem]);
}





//=============================================================================
//		qut_handle_menu_style : Handle the style menu.
//-----------------------------------------------------------------------------
static void
qut_handle_menu_style(GtkWidget *theWidget, gpointer theData)
{	TQ3Uns32		menuItem;



	// Select the style command
	menuItem = (TQ3Uns32) theData;

	Qut_InvokeStyleCommand(menuItem + 1);
}





//=============================================================================
//		qut_handle_menu_special : Handle the special menu.
//-----------------------------------------------------------------------------
static void
qut_handle_menu_special(GtkWidget *theWidget, gpointer theData)
{	TQ3Uns32		menuItem;



	// If the app has a callback, call it
	menuItem = (TQ3Uns32) theData;

	if (gAppMenuSelect != NULL)
		gAppMenuSelect(gView, menuItem);
}





//=============================================================================
//		qut_handle_app_idle : Handle idle events.
//-----------------------------------------------------------------------------
static gint
qut_handle_app_idle(gpointer theData)
{


	// Redraw the image
	Qut_RenderFrame();

	return(TRUE);
}





//=============================================================================
//		qut_handle_window_expose : Handle expose events.
//-----------------------------------------------------------------------------
static gint
qut_handle_window_expose(GtkWidget *theWidget, GdkEventExpose *theEvent, gpointer theData)
{


	// Redraw the image
	Qut_RenderFrame();

	return(TRUE);
}





//=============================================================================
//		qut_handle_window_resize : Handle resize events.
//-----------------------------------------------------------------------------
static gint
qut_handle_window_resize(GtkWidget *theWidget, GtkAllocation *theAllocation, gpointer theData)
{	TQ3DrawContextObject	theDrawContext;
	TQ3Area					thePane;



	// Resize the draw context
	theDrawContext = NULL;
	
	if (gView != NULL)
		Q3View_GetDrawContext(gView, &theDrawContext);

	if (theDrawContext != NULL)
		{
		qut_allocation_to_area(theAllocation, &thePane);
		Q3DrawContext_SetPane(theDrawContext, &thePane);
		Q3Object_Dispose(theDrawContext);
		}

	return(TRUE);
}





//=============================================================================
//		qut_build_renderer_menu : Build the renderer menu.
//-----------------------------------------------------------------------------
static void
qut_build_renderer_menu(void)
{	GtkItemFactoryEntry		factoryItem = { "", NULL, qut_handle_menu_renderer, 0, NULL };
	char					theName[kQ3StringMaximumLength], menuItem[kQ3StringMaximumLength];
	GtkItemFactory			*itemFactory;
	TQ3SubClassData			rendererData;
	TQ3Status				qd3dStatus;
	TQ3Uns32				n, m;



	// Find the menu bar item factory
	itemFactory = gtk_item_factory_from_widget(gMenuBar);
	if (itemFactory == NULL)
		return;



	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;



	// If we can find any renderers, add them to the menu
	if (rendererData.numClasses != 0)
		{
		// First slot is a dummy
		gRenderers[0] = kQ3ObjectTypeInvalid;
		m = 1;
		
		
		// Fill the remaining slots
		for (n = 0; n < rendererData.numClasses; n++)
			{
			// Skip the generic renderer, since it can't actually render
			if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
				{
				// Grab the nick name, falling back to the class name if that fails
				qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n], theName);
				if (qd3dStatus == kQ3Failure || theName[0] == 0x00)
					qd3dStatus = Q3ObjectHierarchy_GetStringFromType(rendererData.classTypes[n], theName);


				// Add the menu item and save the type
				if (qd3dStatus == kQ3Success)
					{
					sprintf(menuItem, "/_Renderer/%s", theName);
						
					factoryItem.path            = menuItem;
					factoryItem.callback_action = m;
					gtk_item_factory_create_items(itemFactory, 1, &factoryItem, NULL);

					gRenderers[m++] = rendererData.classTypes[n];
					}
				}
			}
		}



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}





//=============================================================================
//		qut_build_style_menu : Build the style menu.
//-----------------------------------------------------------------------------
static void
qut_build_style_menu(void)
{	GtkItemFactoryEntry		menuItems[] = { { "/_Style",                                    NULL, NULL,                  0, "<LastBranch>" },
											{ "/Style/Fill Style Filled",                   NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Fill Style Edges",                    NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Fill Style Points",                   NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep1",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Backfacing Style Both",               NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Backfacing Style Remove",             NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Backfacing Style Flip",               NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep2",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Interpolation Style None",            NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Interpolation Style Vertex",          NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Interpolation Style Pixel",           NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep3",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Orientation Style Clockwise",         NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Orientation Style Counter-Clockwise", NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep4",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Anti-Alias Style Off",                NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Anti-Alias Style Edges",              NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Anti-Alias Style Filled",             NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep5",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Fog Style On",                        NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Fog Style Off",                       NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/sep6",                                NULL, NULL,                  0, "<Separator>"  },
											{ "/Style/Subdivision Style Constant (5, 5)",   NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Constant (25, 25)", NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Constant (50, 50)", NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Constant (50, 5)",  NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style World (0.1)",       NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style World (0.5)",       NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style World (2.5)",       NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Screen Space (3)",  NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Screen Space (10)", NULL, qut_handle_menu_style, 0, NULL           } };
											{ "/Style/Subdivision Style Screen Space (30)", NULL, qut_handle_menu_style, 0, NULL           } };
	GtkItemFactory			*itemFactory;
	gint					numItems;



	// Create the menu
	numItems    = sizeof(menuItems) / sizeof(GtkItemFactoryEntry);
	itemFactory = gtk_item_factory_from_widget(gMenuBar);
	if (itemFactory != NULL)
		gtk_item_factory_create_items(itemFactory, numItems, menuItems, NULL);
}





//=============================================================================
//		qut_build_help_menu : Build the help menu.
//-----------------------------------------------------------------------------
static void
qut_build_help_menu(void)
{	GtkItemFactoryEntry		menuItems[] = { { "/_Help",               NULL, NULL,                  0, "<LastBranch>" },
											{ "/_Help/_About Qut...", NULL, qut_handle_menu_about, 0, NULL           } };
	GtkItemFactory			*itemFactory;
	gint					numItems;



	// Create the menu
	numItems    = sizeof(menuItems) / sizeof(GtkItemFactoryEntry);
	itemFactory = gtk_item_factory_from_widget(gMenuBar);
	if (itemFactory != NULL)
		gtk_item_factory_create_items(itemFactory, numItems, menuItems, NULL);
}





//=============================================================================
//		qut_create_menus : Build the menu bar.
//-----------------------------------------------------------------------------
static void
qut_create_menus(GtkWidget *window, GtkWidget **menuBar)
{	GtkItemFactoryEntry		menuItems[] = { { "/_File",       NULL,         NULL,          0, "<Branch>"    },
											{ "/File/_Close", "<control>W", gtk_main_quit, 0, NULL          },
											{ "/File/sep1",   NULL,         NULL,          0, "<Separator>" },
											{ "/File/_Quit",  "<control>Q", gtk_main_quit, 0, NULL          },
											{ "/_Renderer",   NULL,         NULL,          0, "<Branch>"    } };
	GtkItemFactory			*itemFactory;
	GtkAccelGroup			*accelGroup;
	gint					numItems;



	// Create the accelerator group and item factory
	accelGroup  = gtk_accel_group_new();
	itemFactory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<main>", accelGroup);



	// Generate the menu items
	numItems = sizeof(menuItems) / sizeof(GtkItemFactoryEntry);
	gtk_item_factory_create_items(itemFactory, numItems, menuItems, NULL);



	// Attach the accelerator group and return the menu bar
	gtk_window_add_accel_group(GTK_WINDOW(window), accelGroup);
	*menuBar = gtk_item_factory_get_widget(itemFactory, "<main>");
}





//=============================================================================
//      qut_initialize : Initialise ourselves.
//-----------------------------------------------------------------------------
static void
qut_initialize(int argc, char *argv[])
{	TQ3Status	qd3dStatus;



	// Initialise ourselves
 	gtk_init(&argc, &argv);
 
 	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		exit(-1);	

	App_Initialise();

	if (gWindow == NULL)
		exit(-1);	



	// Build the extra menus
	qut_build_renderer_menu();
	qut_build_style_menu();
	qut_build_help_menu();



	// Set up the idle function
	gtk_idle_add(qut_handle_app_idle, NULL);
}





//=============================================================================
//      qut_terminate : Terminate ourselves.
//-----------------------------------------------------------------------------
static void
qut_terminate(void)
{
}





//=============================================================================
//		Public functions.
//-----------------------------------------------------------------------------
//      Qut_CreateWindow : Create the window.
//-----------------------------------------------------------------------------
void
Qut_CreateWindow(const char		*windowTitle,
					TQ3Uns32	theWidth,
					TQ3Uns32	theHeight,
					TQ3Boolean	canResize)
{	GdkVisual	*theVisual;



	// Set up the visual
	theVisual = gdk_gl_choose_visual();
	gtk_widget_push_colormap(gdk_colormap_new(theVisual, TRUE));
	gtk_widget_push_visual(theVisual);



	// Create the window
	gWindow          = (void *) gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gWindowCanResize = canResize;

    gtk_widget_set_usize(GTK_WIDGET(gWindow), theWidth, theHeight);
    gtk_window_set_title(GTK_WINDOW(gWindow), windowTitle);
    gtk_signal_connect(GTK_OBJECT(gWindow), "delete_event", (GtkSignalFunc) gtk_main_quit, NULL);
	gtk_window_set_policy(GTK_WINDOW(gWindow), (gint) canResize, (gint) canResize, FALSE);



	// Create the content widget
	gWindowContent = gtk_vbox_new(FALSE, 1);
	gtk_container_border_width(GTK_CONTAINER(gWindowContent), 1);
	gtk_container_add(GTK_CONTAINER(gWindow), gWindowContent);
	gtk_widget_show(gWindowContent);
	gtk_signal_connect(GTK_OBJECT(gWindowContent), "expose_event",  (GtkSignalFunc) qut_handle_window_expose, NULL);
	gtk_signal_connect(GTK_OBJECT(gWindowContent), "size_allocate", (GtkSignalFunc) qut_handle_window_resize, NULL);



	// Create the menu bar
	qut_create_menus(gWindow, &gMenuBar);
	gtk_box_pack_start(GTK_BOX(gWindowContent), gMenuBar, FALSE, TRUE, 0);
	gtk_widget_show(gMenuBar);



	// Show the window
	gtk_widget_show(gWindow);
}





//=============================================================================
//		Qut_SelectMetafile : Select a metafile.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafile(void)
{	char					thePath[kQ3StringMaximumLength];
	TQ3StorageObject		theStorage;



	// Prompt for the path to the file
	printf("Path to model: ");
//	scanf("%s", thePath);
	gets(thePath);

	if (thePath[0] == 0x00)
		return(NULL);
	
	
	
	// Create a storage object for the file
	theStorage = Q3PathStorage_New(thePath);
	return(theStorage);
}





//=============================================================================
//		Qut_SelectPictureFile : Select a picture file.
//-----------------------------------------------------------------------------
TQ3Status
Qut_SelectPictureFile(void *theFile, TQ3Uns32 fileLen)
{


	// dair, to be implemented
	return(kQ3Failure);
}





//=============================================================================
//		Qut_CreateDrawContext : Create the draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Qut_CreateDrawContext(void)
{	TQ3XDrawContextData		x11DrawContextData;
	TQ3Boolean				resetDrawContext;
	TQ3DrawContextObject	theDrawContext;
	Display					*theDisplay;
	XVisualInfo				*visualInfo;
	Drawable				theDrawable;
	TQ3Status				qd3dStatus;
	TQ3Int32				theScreen;



	// Find the display, screen, drawable, and visual
	theDisplay  = GDK_DISPLAY();
	theScreen   = DefaultScreen(theDisplay);
	theDrawable = GDK_WINDOW_XWINDOW(GTK_WIDGET(gWindowContent)->window);
	visualInfo  = glXChooseVisual(theDisplay, theScreen, gOpenGLAttributes);



	// See if we've got an existing draw context we can reuse. If we
	// do, we grab as much of its state data as we can - this means we
	// wil preserve any changes made by the app's view-configure method.
	resetDrawContext = kQ3True;
	if (gView != NULL)
		{
		qd3dStatus = Q3View_GetDrawContext(gView, &theDrawContext);
		if (qd3dStatus == kQ3Success)
			{
			resetDrawContext = kQ3False;
			Q3DrawContext_GetData(theDrawContext, &x11DrawContextData.contextData);
			Q3Object_Dispose(theDrawContext);
			}
		}



	// Reset the draw context data if required
	if (resetDrawContext)
		{
		// Fill in the draw context data
		x11DrawContextData.contextData.clearImageMethod  = kQ3ClearMethodWithColor;
		x11DrawContextData.contextData.clearImageColor.a = 0.0f;
		x11DrawContextData.contextData.clearImageColor.r = 1.0f;
		x11DrawContextData.contextData.clearImageColor.g = 1.0f;
		x11DrawContextData.contextData.clearImageColor.b = 1.0f;
		x11DrawContextData.contextData.paneState         = kQ3True;
		x11DrawContextData.contextData.maskState		 = kQ3False;	
		x11DrawContextData.contextData.doubleBufferState = kQ3True;
		}



	// Reset the fields which are always updated
	qut_allocation_to_area(&(GTK_WIDGET(gWindowContent)->allocation),
						   &x11DrawContextData.contextData.pane);

	x11DrawContextData.display      = theDisplay;
	x11DrawContextData.drawable     = theDrawable;
	x11DrawContextData.visual       = visualInfo->visual;
	x11DrawContextData.cmap         = None;
	x11DrawContextData.colorMapData = NULL;



	// Create the draw context object
	theDrawContext = Q3XDrawContext_New(&x11DrawContextData);
	return(theDrawContext);
}





//=============================================================================
//      Qut_CreateMenu : Create the Special menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenu(qutFuncAppMenuSelect appMenuSelect)
{	GtkItemFactoryEntry		factoryItem = { "/_Special",  NULL, NULL, 0, "<Branch>" };
	GtkItemFactory			*itemFactory;



	// Create the menu
	itemFactory = gtk_item_factory_from_widget(gMenuBar);
	if (itemFactory != NULL)
		gtk_item_factory_create_items(itemFactory, 1, &factoryItem, NULL);



	// Set the callback
	gAppMenuSelect = appMenuSelect;
}





//=============================================================================
//      Qut_CreateMenuItem : Create a menu item.
//-----------------------------------------------------------------------------
//		Note : For now, we always append menu items to the end of the menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenuItem(TQ3Uns32 itemNum, char *itemText)
{	GtkItemFactoryEntry		factoryItem = { "", NULL, qut_handle_menu_special, 0, NULL };
	char					menuItem[kQ3StringMaximumLength];
	GtkItemFactory			*itemFactory;



	// Create the menu item
	itemFactory = gtk_item_factory_from_widget(gMenuBar);
	if (itemFactory != NULL)
		{
		// Work out what type of item we have
		if (strcmp(itemText, kMenuItemDivider) == 0)
			{
			factoryItem.path      = "/Special/sep";
			factoryItem.item_type = "<Separator>";
			}
		else
			{
			sprintf(menuItem, "/Special/%s", itemText);
			factoryItem.path = menuItem;
			}



		// Insert it
		factoryItem.callback_action = gSpecialItemCount + 1;
		gtk_item_factory_create_items(itemFactory, 1, &factoryItem, NULL);
		gSpecialItemCount++;
		}
}





//=============================================================================
//      main : App entry point.
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{


	// Initialise ourselves
	qut_initialize(argc, argv);



	// Run the app
	gtk_main();



	// Clean up
	App_Terminate();
	qut_terminate();

	return(0);
}
