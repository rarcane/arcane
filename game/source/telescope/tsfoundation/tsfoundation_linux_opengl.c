#include "tsfoundation.h"
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

internal Display *global_display;
internal Window *global_root;
internal Window *global_window;
internal GLXContext *gl_context;

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

internal void *
LinuxLoadOpenGLProcedure(char *name)
{
	return glXGetProcAddress((const GLubyte *)name);
}

internal void *
LinuxInitOpenGL()
{
	int nelements;
	GLXFBConfig *frame_buffer_config = glXChooseFBConfig(global_display, DefaultScreen(global_display), 0, &nelements);

	int attributeList[] = {
		GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None};

	XVisualInfo *vi = glXChooseVisual(global_display, DefaultScreen(global_display), attributeList);

	XSetWindowAttributes wa;
	wa.colormap = XCreateColormap(global_display, RootWindow(global_display, vi->screen), vi->visual, AllocNone);
	wa.border_pixel = 0;
	wa.event_mask = StructureNotifyMask;

	Window win = XCreateWindow(global_display,
							   RootWindow(global_display, vi->screen),
							   0,
							   0,
							   TS_APP_DEFAULT_WINDOW_WIDTH,
							   TS_APP_DEFAULT_WINDOW_HEIGHT,
							   0,
							   vi->depth,
							   InputOutput,
							   vi->visual,
							   CWBorderPixel | CWColormap | CWEventMask,
							   &wa);

	XMapWindow(global_display, win);

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte *)"glXCreateContextAttribsARB");
	int attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		0};

	GLXContext ctx = glXCreateContextAttribsARB(global_display, *frame_buffer_config, 0, 1, attribs);
	glXMakeCurrent(global_display, win, ctx);
}

internal void
LinuxOpenGLRefreshScreen(void)
{
	glXSwapBuffers(global_display, global_window);
}
