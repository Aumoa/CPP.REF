// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Threading/Thread.h"
#include "Platform/PlatformProcess.h"
#include "Platform/PlatformAtomics.h"
#include "AssertionMacros.h"
#include <thread>

namespace Ayla
{
	Thread::Thread() noexcept
	{
	}

	Thread::Thread(const Thread& Thr) noexcept
		: Impl(Thr.Impl)
	{
		if (Impl)
		{
			PlatformAtomics::InterlockedIncrement(&Impl->Refs);
		}
	}

	Thread::Thread(Thread&& Thr) noexcept
	{
		std::swap(Impl, Thr.Impl);
	}

	Thread::~Thread() noexcept
	{
		if (Impl)
		{
			if (PlatformAtomics::InterlockedDecrement(&Impl->Refs) == 0)
			{
				PlatformProcess::DestroyCurrentThreadHandle(Impl->Handle);
				delete Impl;
			}
		}
	}

	void Thread::SetDescription(String InDescription) noexcept
	{
		check(IsValid());
		if (Impl)
		{
			PlatformProcess::SetThreadDescription(Impl->Handle, InDescription);
		}
	}

	Thread& Thread::operator =(const Thread& Thr) noexcept
	{
		Impl = Thr.Impl;
		if (Impl)
		{
			PlatformAtomics::InterlockedIncrement(&Impl->Refs);
		}
		return *this;
	}

	Thread& Thread::operator =(Thread&& Thr) noexcept
	{
		if (Impl)
		{
			if (PlatformAtomics::InterlockedDecrement(&Impl->Refs) == 0)
			{
				PlatformProcess::DestroyCurrentThreadHandle(Impl->Handle);
				delete Impl;
			}
		}
		Impl = nullptr;
		std::swap(Impl, Thr.Impl);
		return *this;
	}

	size_t Thread::GetHardwareConcurrency() noexcept
	{
		return (size_t)std::thread::hardware_concurrency();
	}

	Thread Thread::GetCurrentThread()
	{
		static thread_local Thread LocalCache = []()
			{
				Thread Thr;
				Thr.Impl = new Impl_t();
				Thr.Impl->Handle = PlatformProcess::AllocateCurrentThreadHandle();
				Thr.Impl->Refs = 1;
				return Thr;
			}();

		return LocalCache;
	}
}