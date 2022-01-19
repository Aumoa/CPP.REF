// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "NonCopyable.h"
#include <string>
#include <future>
#include <functional>

class CORESOBJECT_API Thread : public NonCopyable
{
	void* ThreadHandle = nullptr;
	int64 ThreadId = 0;
	std::wstring FriendlyName;
	bool bIsManaged = false;

private:
	Thread();

public:
	~Thread();

	void SetFriendlyName(std::wstring_view InFriendlyName);
	std::wstring GetFriendlyName();
	int64 GetThreadId();
	bool IsManaged();

	void SuspendThread();
	void ResumeThread();

	template<class TReturn>
	static std::future<TReturn> NewThread(std::wstring FriendlyName, std::function<TReturn()> Body)
	{
		auto ReturnFuture = std::make_shared<std::promise<TReturn>>();
		Internal_NewThread(std::move(FriendlyName), [ReturnFuture, Body = std::move(Body)]()
		{
			if constexpr (std::same_as<TReturn, void>)
			{
				Body();
				ReturnFuture->set_value();
			}
			else
			{
				TReturn ReturnValue = Body();
				ReturnFuture->set_value(ReturnValue);
			}
		});
		return ReturnFuture->get_future();
	}

private:
	static void Internal_NewThread(std::wstring FriendlyName, std::function<void()> Body);

public:
	static Thread* GetCurrentThread();
};