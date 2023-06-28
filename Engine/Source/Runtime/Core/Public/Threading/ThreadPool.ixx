// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:ThreadPool;

export import Core.Std;

export class CORE_API ThreadPool
{
public:
	using Work_t = std::function<void()>;

public:
	static void BindHandle(void* NativeHandle);
	static void QueueUserWorkItem(Work_t InWork);
	static void QueueDelayedUserWorkItem(std::chrono::nanoseconds InDur, Work_t InWork);
};