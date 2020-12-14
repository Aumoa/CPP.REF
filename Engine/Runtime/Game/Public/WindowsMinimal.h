// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

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