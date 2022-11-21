// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "IO/_win_iocp_impl.h"
#include "IO/IOCompletionOverlapped.h"
#include "Exceptions/SystemException.h"

IOContext::_iocp_impl::_iocp_impl(size_t threadsHint)
{
	_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, threadsHint == -1 ? MAXDWORD : (DWORD)threadsHint);
	if (_iocp == NULL)
	{
		throw SystemException(GetLastError());
	}
}

IOContext::_iocp_impl::~_iocp_impl() noexcept
{
	if (_iocp != NULL)
	{
		// IOCP handle need not to close.
		_iocp = NULL;
	}
}

bool IOContext::_iocp_impl::poll_one()
{
	ULONG_PTR lpKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	DWORD dwTransferred = 0;
	BOOL bSucceeded = GetQueuedCompletionStatus(_iocp, &dwTransferred, &lpKey, &lpOverlapped, INFINITE);

	if (lpOverlapped == NULL)
	{
		return false;
	}

	auto* block = IOCompletionOverlapped::FromOverlapped(lpOverlapped);
	if (bSucceeded)
	{
		block->Complete((size_t)dwTransferred);
	}
	else
	{
		block->Failed(GetLastError());
	}

	return true;
}

void IOContext::_iocp_impl::bind_handle(void* handle)
{
	HANDLE hIOCP = CreateIoCompletionPort((HANDLE)handle, _iocp, (ULONG_PTR)handle, 0);
	if (hIOCP == NULL)
	{
		throw SystemException(GetLastError());
	}
}

void IOContext::_iocp_impl::post(std::function<void(size_t, int32)> work)
{
	auto* ptr = new IOCompletionOverlapped(std::move(work));
	if (PostQueuedCompletionStatus(_iocp, 0, (ULONG_PTR)ptr, reinterpret_cast<LPOVERLAPPED>(ptr->ToOverlapped())) == FALSE)
	{
		throw SystemException(GetLastError());
	}
}

#endif