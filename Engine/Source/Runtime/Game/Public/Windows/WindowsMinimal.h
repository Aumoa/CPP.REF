﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#ifdef GAME_BUILD

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

#pragma warning(push)
#pragma warning(disable: 4005)
#include <Windows.h>
#include <crtdbg.h>
#include <wincodec.h>
#pragma warning(pop)

#undef CreateWindow
#undef OpenEvent

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

using HANDLE = void*;
using HWND = void*;
using UINT = uint32;
using WPARAM = uint64;
using LPARAM = int64;
using LRESULT = int64;
using INT = int32;
using HINSTANCE = void*;
using LPSTR = char*;

#endif