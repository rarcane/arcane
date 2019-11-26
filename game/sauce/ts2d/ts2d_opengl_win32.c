/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2019
*/

global HGLRC global_opengl_render_context;

void *
Win32LoadOpenGLProcedure(char *name)
{
    void *p = (void *)wglGetProcAddress(name);
    if(!p || p == (void *)0x1 || p == (void *)0x2 || p == (void *)0x3 || p == (void *)-1)
    {
        return 0;
    }
    else
    {
        return p;
    }
}

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

void
Win32LoadWGLFunctions(HINSTANCE h_instance)
{
    wglChoosePixelFormatARB    = (PFNWGLCHOOSEPIXELFORMATARBPROC)    Win32LoadOpenGLProcedure("wglChoosePixelFormatARB");
    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) Win32LoadOpenGLProcedure("wglCreateContextAttribsARB");
    wglMakeContextCurrentARB   = (PFNWGLMAKECONTEXTCURRENTARBPROC)   Win32LoadOpenGLProcedure("wglMakeContextCurrentARB");
    wglSwapIntervalEXT         = (PFNWGLSWAPINTERVALEXTPROC)         Win32LoadOpenGLProcedure("wglSwapIntervalEXT");
}

b32
Win32InitOpenGL(HDC *device_context, HINSTANCE h_instance)
{
    b32 result = 0;
    
    // NOTE(Ryan): Set up pixel format for dummy context
    int pixel_format = 0;
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,
        8,
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    pixel_format = ChoosePixelFormat(*device_context, &pfd);
    
    if(pixel_format)
    {
        SetPixelFormat(*device_context, pixel_format, &pfd);
        HGLRC gl_dummy_render_context = wglCreateContext(*device_context);
        wglMakeCurrent(*device_context, gl_dummy_render_context);
        
        Win32LoadWGLFunctions(h_instance);
        
        // NOTE(rjf): Setup real pixel format
        {
            int pf_attribs_i[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB, 32,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,
                0};
            
            UINT num_formats = 0;
            wglChoosePixelFormatARB(*device_context,
                                    pf_attribs_i,
                                    0,
                                    1,
                                    &pixel_format,
                                    &num_formats);
        }
        
        if(pixel_format)
        {
            const int context_attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                0};
            
            global_opengl_render_context = wglCreateContextAttribsARB(*device_context,
                                                                      gl_dummy_render_context,
                                                                      context_attribs);
            if(global_opengl_render_context)
            {
                wglMakeCurrent(*device_context, 0);
                wglDeleteContext(gl_dummy_render_context);
                wglMakeCurrent(*device_context, global_opengl_render_context);
                wglSwapIntervalEXT(0);
                result = 1;
            }
        }
    }
    
    return result;
}

void
Win32CleanUpOpenGL(HDC *device_context)
{
    wglMakeCurrent(*device_context, 0);
    wglDeleteContext(global_opengl_render_context);
}

void
Win32OpenGLSwapBuffers(void)
{
    wglSwapLayerBuffers(global_device_context, WGL_SWAP_MAIN_PLANE);
}

void
Ts2dInitWin32(Ts2d *renderer)
{
    if(!Win32InitOpenGL(&global_device_context, global_instance_handle))
    {
        Win32OutputError("Fatal Error", "OpenGL context initialization failure.");
        platform->quit = 1;
    }
    renderer->SwapBuffers = Win32OpenGLSwapBuffers;
    renderer->LoadOpenGLProcedure = Win32LoadOpenGLProcedure;
}