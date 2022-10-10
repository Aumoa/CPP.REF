// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "TimeSpan.h"
#include "Misc/String.h"
#include "Threading/Tasks/Task.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include <functional>
#include <mutex>
#include <condition_variable>
#include <memory>

class CORE_API Thread
{
public:
	struct _Impl;
	
private:
	std::shared_ptr<_Impl> _impl;

private:
	Thread(std::shared_ptr<_Impl> impl) noexcept;

public:
	Thread() noexcept;
	Thread(const Thread& rhs) noexcept;
	Thread(Thread&& rhs) noexcept;
	~Thread() noexcept;

	void SetFriendlyName(const String& friendlyName) noexcept;
	String GetFriendlyName() const noexcept;

	void SuspendThread(const TimeSpan& waitFor = 0s) const noexcept;
	void ResumeThread() const noexcept;

	Task<> JoinAsync() const noexcept;

	int32 GetThreadId() const noexcept;
	bool IsManaged() const noexcept;
	void* GetNativeHandle() const noexcept;
	bool IsValid() const noexcept;

public:
	Thread& operator =(const Thread& rhs) noexcept;
	Thread& operator =(Thread&& rhs) noexcept;

public:
	static Thread CreateThread(const String& friendlyName, std::function<void()> entry);
	static Thread GetCurrentThread();
};