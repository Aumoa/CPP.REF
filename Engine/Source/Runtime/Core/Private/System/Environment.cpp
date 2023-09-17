// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "System/Environment.h"
#include "IO/File.h"
#include "System/Path.h"
#include "System/InvalidOperationException.h"

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