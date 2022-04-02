// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Thread.h"
#include "Misc/Exceptions.h"
#include "Misc/String.h"

using namespace libty;

Thread::ThreadSuspendToken::ThreadSuspendToken(Thread* CurrentThread)
	: CurrentThread(CurrentThread)
{
}

std::future<void> Thread::ThreadSuspendToken::Suspend()
{
	if (SuspendPromise.has_value())
	{
		throw invalid_operation("Thread already wait for suspend.");
	}
	return SuspendPromise.emplace().get_future();
}

void Thread::ThreadSuspendToken::Resume()
{
	if (SuspendPromise.has_value())
	{
		SuspendPromise.reset();
		CurrentThread->ResumeThread();
	}
}

void Thread::ThreadSuspendToken::Join()
{
	if (!SuspendPromise.has_value())
	{
		return;
	}

	SuspendPromise->set_value();
	CurrentThread->SuspendThread();
}