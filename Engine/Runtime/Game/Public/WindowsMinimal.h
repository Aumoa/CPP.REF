// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_GAME_API__

#undef interface

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NODRAWTEXT
#define NOWH
#define NOKANJI
#define NOMCX
#define NOHELP

#include <Windows.h>
#include <crtdbg.h>

#ifndef interface
#define interface struct
#endif

#undef CreateWindow

#else

#ifndef CALLBACK
#define CALLBACK __stdcall
#endif

#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef _In_
#define _In_
#endif

#ifndef _In_opt_
#define _In_opt_
#endif

using HWND = void*;
using UINT = uint32;
using WPARAM = uint64;
using LPARAM = int64;
using LRESULT = int64;
using INT = int32;
using HINSTANCE = void*;
using LPSTR = char*;

#endif