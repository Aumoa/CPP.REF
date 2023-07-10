// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:SynchronizationContext;

export import Core.System;
export import :Spinlock;

export class CORE_API SynchronizationContext
{
private:
	bool bFinalized = false;
	std::atomic<size_t> NumWorks;
	std::queue<std::function<void()>> Works;
	Spinlock Lock;

public:
	SynchronizationContext() = default;

	static std::shared_ptr<SynchronizationContext> Install();
	static void Uninstall(std::shared_ptr<SynchronizationContext> Context);
	static std::shared_ptr<SynchronizationContext> GetCurrent();

	void Enqueue(std::function<void()> Work);
	inline bool IsQueued() const { return NumWorks > 0; }
	std::function<void()> Dequeue();
};