// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Environment.h"
#include "IO/File.h"
#include "Path.h"
#include "InvalidOperationException.h"
#include "Platform/PlatformProcess.h"
#include <stdlib.h>

namespace Ayla
{
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
}