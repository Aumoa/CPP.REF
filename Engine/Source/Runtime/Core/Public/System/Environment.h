// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "System/String.h"
#include "System/AssertionMacros.h"

struct CORE_API Environment : public StaticClass
{
	static String GetCurrentDirectory()
	{
		return String(std::filesystem::current_path().wstring());
	}

	static void SetCurrentDirectory(String InPath)
	{
		std::filesystem::current_path(InPath.path());
	}

	static String GetEngineDirectory()
	{
		check(EngineDirectory.IsEmpty() == false);
		return EngineDirectory;
	}

	static void SetEngineDirectory(String InPath)
	{
		EngineDirectory = InPath;
	}

private:
	static String EngineDirectory;
};