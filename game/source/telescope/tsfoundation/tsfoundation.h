/*
* Copyright (C) Ryan Fleury - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Ryan Fleury <ryan.j.fleury@gmail.com>, 2020
*/

#ifndef TSFOUNDATION_H_INCLUDED
#define TSFOUNDATION_H_INCLUDED
#define TSFOUNDATION 1

// NOTE(rjf): Platform Headers
#if BUILD_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// NOTE(rjf): C Standard Library
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#define TS_DOC_DESC(desc)
#define TS_DOC_MEMBER(desc)
#define TS_DOC_PARAM(desc)

#if BUILD_WIN32
#define TS_PLATFORM_WIN32 1
#define TS_PLATFORM TS_PLATFORM_WIN32
#elif BUILD_LINUX
#define TS_PLATFORM_LINUX 1
#define TS_PLATFORM TS_PLATFORM_LINUX
#endif

// NOTE(rjf): Program Options
#ifndef TSFOUNDATION_PROGRAM_OPTIONS
#define TSFOUNDATION_PROGRAM_OPTIONS "tsfoundation_program_options_default.h"
#endif // TSFOUNDATION_PROGRAM_OPTIONS
#include TSFOUNDATION_PROGRAM_OPTIONS

// NOTE(rjf): Platform Defines for Telescope Modules
#if TS_PLATFORM == TS_PLATFORM_WIN32
#define TS3D_WIN32 1
#define TS2D_WIN32 1
#define TSASSETS_WIN32 1
#elif TS_PLATFORM == TS_PLATFORM_LINUX
#define TS3D_LINUX 1
#define TS2D_LINUX 1
#define TSASSETS_LINUX 1
#endif

// NOTE(rjf): Alignment
#if _MSC_VER
#define Align(n) __declspec(align(n))
#else
#define Align(n) __attribute__((aligned (n)))
#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

// NOTE(rjf): Build-modes for team roles
#define ROLE_PROGRAMMER 1
#define ROLE_ARTIST     2

#define Stringify(a) _Stringify(a)
#define _Stringify(a) #a

#define global        static
#define internal      static
#define local_persist static

#define ArrayCount(a)              (sizeof(a) / sizeof(a[0]))
#define StructMemberOffset(st, m)  ((u32) & (((st *)0)->m))
#define ObjectMemberOffset(o, m)   (u32)((u8 *)(&((o).m)) - (u8 *)(&(o)))
#define IsOdd(i)                   ((i)&0x01)
#define IsEven(i)                  (!(IsOdd(i)))
#define Fraction(f)                ((f) - (int)(f))

#define Bytes(n)      (n)
#define Kilobytes(n)  (Bytes(n) * 1024)
#define Megabytes(n)  (Kilobytes(n) * 1024)
#define Gigabytes(n)  (Megabytes(n) * 1024)

#define CalculateCStringLength(s)  ((u32)strlen(s))

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef float     real32;
typedef double    real64;
typedef float     float32;
typedef double    float64;
typedef int8      bool8;
typedef int16     bool16;
typedef int32     bool32;
typedef int64     bool64;

typedef int8      i8;
typedef int16     i16;
typedef int32     i32;
typedef int64     i64;
typedef uint8     u8;
typedef uint16    u16;
typedef uint32    u32;
typedef uint64    u64;
typedef real32    r32;
typedef real64    r64;
typedef float32   f32;
typedef float64   f64;
typedef bool8     b8;
typedef bool16    b16;
typedef bool32    b32;
typedef bool64    b64;

#ifndef TSFOUNDATION_MODULES_DECLARATION_FILE
#define TSFOUNDATION_MODULES_DECLARATION_FILE "tsfoundation_modules_declaration_default.h"
#endif

#include "tsfoundation_memory.h"
#include "tsfoundation_math.h"
#include "tsfoundation_strings.h"
#include "tsfoundation_debug.h"
#include "tsfoundation_platform.h"
#include "tsfoundation_project.h"
#include "tsfoundation_debug.c"

#endif // TSFOUNDATION_H_INCLUDED