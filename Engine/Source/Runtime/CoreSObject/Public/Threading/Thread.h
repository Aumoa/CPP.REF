// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "NonCopyable.h"
#include "SuspendToken.h"
#include <string>
#include <future>
#include <functional>

class CORESOBJECT_API Thread : public NonCopyable
{
public:
	class CORESOBJECT_API ThreadSuspendToken : public SuspendToken
	{
		friend class Thread;
		Thread* CurrentThread;

		std::optional<std::promise<void>> SuspendPromise;

	private:
		ThreadSuspendToken(Thread* CurrentThread);

	public:
		virtual std::future<void> Suspend() override;
		virtual void Resume() override;

		void Join();
	};

	void* ThreadHandle = nullptr;
	int64 ThreadId = 0;
	std::wstring FriendlyName;
	bool bIsManaged = false;
	ThreadSuspendToken* SToken = nullptr;

	std::promise<void> JoinPromise;
	std::future<void> JoinFuture;

private:
	Thread();

public:
	~Thread();

	void SetFriendlyName(std::wstring_view InFriendlyName);
	void SuspendThread();
	void ResumeThread();
	void Join();

	std::wstring GetFriendlyName() const;
	int64 GetThreadId() const;
	bool IsManaged() const;
	ThreadSuspendToken* GetSuspendToken() const;

	static Thread* CreateThread(std::wstring_view FriendlyName, std::function<void()> ThreadEntry);

public:
	static Thread* GetCurrentThread();
};