// Copyright 2020 Aumoa.lib. All right reserved.

#include "Program.h"

#include "Application.h"
#include "THGameInstance.h"
#include "PlatformMisc/PlatformConsole.h"

using namespace std;

int32 Program::Main()
{
	PlatformConsole::Alloc();

	auto app = NewObject<Application>();
	int32 ret = app->Run<THGameInstance>();

	PlatformConsole::Free();
	return ret;
}