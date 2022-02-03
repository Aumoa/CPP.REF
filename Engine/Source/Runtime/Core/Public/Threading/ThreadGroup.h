// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <chrono>

class CORE_API ThreadGroup
{
	struct Impl;
	class ThreadGroupSuspendToken;

private:
	Impl* _Impl;
	ThreadGroupSuspendToken* SuspendToken = nullptr;
	std::wstring GroupName;

public:
	ThreadGroup(std::wstring_view GroupName, size_t NumThreads = 0);
	virtual ~ThreadGroup() noexcept;

	void Run(std::function<void()> Body);
	void Delay(std::chrono::milliseconds Timeout, std::function<void()> Body);

private:
	void Worker(size_t Index);
};