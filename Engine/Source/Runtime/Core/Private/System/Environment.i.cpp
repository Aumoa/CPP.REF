// Copyright 2020-2022 Aumoa.lib. All right reserved.

import Core;

#include <stdlib.h>

String Environment::EngineDirectory;

void Environment::Init()
{
	static int Trap_init = (SetupEngineDirectory(), 0);
}

void Environment::SetupEngineDirectory()
{
	String Directory = PlatformProcess::FindEngineDirectory();
	if (Directory.IsEmpty() == false)
	{
		EngineDirectory = Directory;
	}
}

void Environment::SetEnvironmentVariable(String InName, String InValue)
{
	if (PlatformProcess::SetEnvironmentVariable(InName, InValue) == false)
	{
		throw InvalidOperationException();
	}
}

String Environment::GetEnvironmentVariable(String InName)
{
	// GetEnvironmentVariable is return literal capture.
	return PlatformProcess::GetEnvironmentVariable(InName).Clone();
}