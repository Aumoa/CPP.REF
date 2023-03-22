// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API CommandLine
{
	CommandLine() = delete;

private:
	static std::vector<String> Args;

public:
	static void Set(int Argc, const char* const Argv[]);
	static bool TryGetValue(String InName, String& OutValue);

	static bool TryGetValue(String InName)
	{
		String _;
		return TryGetValue(InName, _);
	}
};