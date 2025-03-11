// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "System/StaticClass.h"
#include "System/String.h"
#include "System/AssertionMacros.h"

struct CORE_API Environment : public StaticClass
{
	static String GetCurrentDirectory()
	{
		Init();
		return String(std::filesystem::current_path().wstring());
	}

	static void SetCurrentDirectory(String InPath)
	{
		Init();
		std::filesystem::current_path(InPath.path());
	}

	static String GetEngineDirectory()
	{
		Init();
		check(EngineDirectory.IsEmpty() == false);
		return EngineDirectory;
	}

	static void SetEnvironmentVariable(String InName, String InValue);
	static String GetEnvironmentVariable(String InName);

	static constexpr String NewLine = PLATFORM_NEWLINE;

private:
	static void Init();
	static void SetupEngineDirectory();

private:
	static String EngineDirectory;
};