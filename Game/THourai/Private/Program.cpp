// Copyright 2020 Aumoa.lib. All right reserved.

#include "Program.h"

#include "Application.h"
#include "THGameInstance.h"

int32 Program::Main()
{
	auto app = NewObject<Application>();
	return app->Run<THGameInstance>();
}