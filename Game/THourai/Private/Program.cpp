// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Program.h"

#include "Windows/Application.h"
#include "Windows/CoreWindow.h"
#include "THGameInstance.h"
#include "PlatformMisc/PlatformConsole.h"

using namespace std;

int32 Program::Main()
{
#if defined(_DEBUG)
	PlatformConsole::Alloc();
#endif

	auto gameInstance = NewObject<THGameInstance>();
	gameInstance->Run(NewObject<CoreWindow>(gameInstance.Get()));

#if defined(_DEBUG)
	PlatformConsole::Free();
#endif
	return 0;
}