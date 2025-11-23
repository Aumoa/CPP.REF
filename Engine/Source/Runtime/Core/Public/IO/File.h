// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "Threading/Tasks/Task.h"

namespace Ayla
{
	class CORE_API File : public StaticClass
	{
	public:
		static Task<String> ReadAllTextAsync(String InPath, std::stop_token InCancellationToken = {});
		static String ReadAllText(String InPath);
		static Task<> WriteAllTextAsync(String InPath, String InContent, std::stop_token InCancellationToken = {});
		static Task<bool> CompareAndWriteAllTextAsync(String InPath, String InContent, std::stop_token InCancellationToken = {});
		static bool Exists(String InPath);
		static void Delete(String InPath);
	};
}