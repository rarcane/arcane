/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

global GLXContext global_gl_context;

internal void *
LinuxLoadOpenGLProcedure(char *name)
{
    void *addr = glXGetProcAddressARB((const GLubyte *)name);
    return addr;
}

b32
LinuxInitOpenGL(void)
{
    b32 result = 0;
    
    GLint gl_attributes[] =
    {
        GLX_RENDER_TYPE     , GLX_RGBA_BIT,
        GLX_RED_SIZE        , 8,
        GLX_GREEN_SIZE      , 8,
        GLX_BLUE_SIZE       , 8,
        GLX_ALPHA_SIZE      , 8,
        GLX_DEPTH_SIZE      , 24,
        GLX_DOUBLEBUFFER    , True,
        None
    };
    
    int screen = DefaultScreen(global_display);
    int element_count;
    GLXFBConfig *framebuffer_config = glXChooseFBConfig(global_display, screen, gl_attributes, &element_count);
    if(!framebuffer_config)
    {
        goto quit;
    }
    
    XVisualInfo *visual_info = glXChooseVisual(global_display, screen, gl_attributes);
    
    if(!visual_info)
    {
        goto quit;
    }
    
    Colormap color_map = XCreateColormap(global_display,
                                         global_root,
                                         visual_info->visual,
                                         AllocNone);
    
    XSetWindowAttributes set_window_attributes = {0};
    set_window_attributes.colormap = color_map;
    set_window_attributes.event_mask = (KeyPressMask      |
                                        KeyReleaseMask    |
                                        PointerMotionMask |
                                        ButtonPressMask   |
                                        ButtonReleaseMask);
    
    const char *glx_extensions = glXQueryExtensionsString(global_display, screen);
    
    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = 
        (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");
    
    global_window = XCreateWindow(global_display, global_root,
                                  0, 0, TS_APP_DEFAULT_WINDOW_WIDTH, TS_APP_DEFAULT_WINDOW_HEIGHT, 0, visual_info->depth,
                                  InputOutput, visual_info->visual, CWColormap | CWEventMask, &set_window_attributes);
    
    
    GLint gl3_attributes[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
        None
    };
    
    gl_context = glXCreateContextAttribsARB(global_display, *framebuffer_config,
                                            0, GL_TRUE, gl3_attributes);
    
    if(!gl_context)
    {
        LinuxOutputError("Fatal Error", "OpenGL context initialization error.");
        goto quit;
    }
    
    XFree(visual_info);
    
    if(!glXMakeCurrent(global_display, global_window, gl_context))
    {
        LinuxOutputError("Fatal Error", "OpenGL context could not be made current.");
        goto quit;
    }
    
    result = 1;
    quit:;
    return result;
}

void
LinuxCleanUpOpenGL(void)
{}

void
LinuxOpenGLSwapBuffers(void)
{
    glXSwapBuffers(global_display, global_window);
}

void
Ts2dInitLinux(Ts2d *renderer)
{
    if(!LinuxInitOpenGL())
    {
        LinuxOutputError("Fatal Error", "OpenGL context initialization failure.");
        platform->quit = 1;
    }
    renderer->SwapBuffers = LinuxOpenGLSwapBuffers;
    renderer->LoadOpenGLProcedure = LinuxLoadOpenGLProcedure;
}