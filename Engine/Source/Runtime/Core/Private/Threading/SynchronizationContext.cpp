// Copyright 2020-2023 Aumoa.lib. All right reserved.

import Core.System;
import Core.Threading;

thread_local std::weak_ptr<SynchronizationContext> ThreadContext;

std::shared_ptr<SynchronizationContext> SynchronizationContext::Install()
{
	auto Context = std::make_shared<SynchronizationContext>();
	ThreadContext = Context;
	return Context;
}

void SynchronizationContext::Uninstall(std::shared_ptr<SynchronizationContext> Context)
{
	Context->bFinalized = true;
}

std::shared_ptr<SynchronizationContext> SynchronizationContext::GetCurrent()
{
	return ThreadContext.lock();
}

void SynchronizationContext::Enqueue(std::function<void()> Work)
{
	std::unique_lock ScopedLock(Lock);
	Works.emplace(std::move(Work));
	++NumWorks;
	ThreadPool::NotifyCustom();
}

std::function<void()> SynchronizationContext::Dequeue()
{
	std::unique_lock ScopedLock(Lock);
	if (Works.empty() == false)
	{
		std::function<void()> Return = std::move(Works.front());
		Works.pop();
		return Return;
	}
	return nullptr;
}