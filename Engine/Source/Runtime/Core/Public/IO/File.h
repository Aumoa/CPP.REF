// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "System/StaticClass.h"
#include "Threading/Tasks/Task.h"

class CORE_API File : public StaticClass
{
public:
	static Task<String> ReadAllTextAsync(String InPath, CancellationToken InCancellationToken = {});
	static String ReadAllText(String InPath);
	static Task<> WriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken = {});
	static Task<bool> CompareAndWriteAllTextAsync(String InPath, String InContent, CancellationToken InCancellationToken = {});
	static bool Exists(String InPath);
	static void Delete(String InPath);
};