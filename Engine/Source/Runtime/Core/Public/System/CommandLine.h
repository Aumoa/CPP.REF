// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <vector>
#include "System/String.h"

class CORE_API CommandLine
{
	CommandLine() = delete;

private:
	static std::vector<String> Args;

public:
	static void Init(int Argc, const char* const Argv[]);
	static void Init(String CmdArgs);
	static bool TryGetValue(String InName, String& OutValue);
	static bool Contains(String InName);
};