// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Program.h"

#include "Application.h"
#include "THGameInstance.h"
#include "PlatformMisc/PlatformConsole.h"

using namespace std;

int32 Program::Main()
{
#if defined(_DEBUG)
	PlatformConsole::Alloc();
#endif

	auto app = NewObject<Application>();
	int32 ret = app->Run<THGameInstance>();

#if defined(_DEBUG)
	PlatformConsole::Free();
#endif
	return ret;
}