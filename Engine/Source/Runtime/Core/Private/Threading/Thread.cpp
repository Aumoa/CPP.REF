// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Thread.h"
#include <future>

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>
#undef GetObject

#pragma pop_macro("TEXT")

struct Thread::_Impl
{
	HANDLE _localHandle = nullptr;
	HANDLE _threadHandle = nullptr;
	DWORD _threadId = 0;

	TaskCompletionSource<> _joinSrc = TaskCompletionSource<>::Create();
	std::mutex _suspendLock;
	std::condition_variable _suspendVar;

	bool _isManaged = false;

	_Impl()
	{
		_localHandle = ::GetCurrentThread();
		_threadId = ::GetThreadId(_localHandle);
		_threadHandle = ::OpenThread(GENERIC_ALL, TRUE, _threadId);
	}

	~_Impl() noexcept
	{
		if (_threadHandle)
		{
			::CloseHandle(_threadHandle);
			_threadHandle = nullptr;
		}
	}

	String _Impl_get_thread_name() const noexcept
	{
		PWSTR pwsz = nullptr;
		if (SUCCEEDED(::GetThreadDescription(_threadHandle, &pwsz)))
		{
			return String::FromLiteral(pwsz);
		}
		else
		{
			return TEXT("");
		}
	}

	bool _Impl_set_thread_name(const String& threadName) noexcept
	{
		return SUCCEEDED(::SetThreadDescription(_threadHandle, (const wchar_t*)threadName));
	}

	void* _Get_native_handle() const noexcept
	{
		return _threadHandle;
	}
};

#endif

Thread::Thread(std::shared_ptr<_Impl> impl) noexcept
	: _impl(std::move(impl))
{
}

Thread::Thread() noexcept
{
}

Thread::Thread(const Thread& rhs) noexcept
	: _impl(rhs._impl)
{
}

Thread::Thread(Thread&& rhs) noexcept
	: _impl(std::move(rhs._impl))
{
}

Thread::~Thread() noexcept
{
}

void Thread::SetFriendlyName(const String& friendlyName) noexcept
{
	_impl->_Impl_set_thread_name(friendlyName);
}

String Thread::GetFriendlyName() const noexcept
{
	return _impl->_Impl_get_thread_name();
}

void Thread::SuspendThread(const TimeSpan& waitFor) const noexcept
{
	std::unique_lock lock(_impl->_suspendLock);
	if (waitFor > 0s)
	{
		_impl->_suspendVar.wait_for(lock, (std::chrono::milliseconds)waitFor);
	}
	else
	{
		_impl->_suspendVar.wait(lock);
	}
}

void Thread::ResumeThread() const noexcept
{
	std::unique_lock lock(_impl->_suspendLock);
	_impl->_suspendVar.notify_all();
}

Task<> Thread::JoinAsync() const noexcept
{
	return _impl->_joinSrc.GetTask();
}

int32 Thread::GetThreadId() const noexcept
{
	return _impl->_threadId;
}

bool Thread::IsManaged() const noexcept
{
	return _impl->_isManaged;
}

void* Thread::GetNativeHandle() const noexcept
{
	return _impl->_Get_native_handle();
}

Thread& Thread::operator =(const Thread& rhs) noexcept
{
	_impl = rhs._impl;
	return *this;
}

Thread& Thread::operator =(Thread&& rhs) noexcept
{
	_impl = std::move(rhs._impl);
	return *this;
}

Thread Thread::CreateThread(const String& friendlyName, std::function<void()> entry)
{
	std::promise<Thread> promise;
	std::future<Thread> future = promise.get_future();

	std::thread([&, entryPoint = entry]()
	{
		Thread thread = GetCurrentThread();
		thread.SetFriendlyName(friendlyName);
		thread._impl->_isManaged = true;

		promise.set_value(thread);
		try
		{
			entryPoint();
		}
		catch (...)
		{
			thread._impl->_joinSrc.SetException(std::current_exception());
			return;
		}

		thread._impl->_joinSrc.SetResult();
	}).detach();

	return future.get();
}

Thread Thread::GetCurrentThread()
{
	static thread_local std::shared_ptr<_Impl> impl = std::make_shared<Thread::_Impl>();
	return Thread(impl);
}