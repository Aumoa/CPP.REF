// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "Threading/Tasks/Task.h"

class CORE_API File : public StaticClass
{
public:
	static Task<String> ReadAllTextAsync(String InPath, std::stop_token InCancellationToken = {});
	static Task<> WriteAllTextAsync(String InPath, String InContent, std::stop_token InCancellationToken = {});
	static bool Exists(String InPath);
	static void Delete(String InPath);
};