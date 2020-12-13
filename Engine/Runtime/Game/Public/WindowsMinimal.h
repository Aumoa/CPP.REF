// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#undef interface

int __cdecl memcmp(void const* _Buf1, void const* _Buf2, size_t _Size);
int __cdecl memset(void* _Dst, int _Val, size_t _Size);
void __cdecl memcpy(void* _Dst, void const* _Src, size_t _Size);
int __cdecl _wcsicmp(wchar_t const* _String1, wchar_t const* _String2);

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

#undef CreateWindow