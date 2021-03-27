// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

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