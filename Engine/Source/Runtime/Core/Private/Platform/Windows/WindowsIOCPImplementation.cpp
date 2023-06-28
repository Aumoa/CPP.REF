// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsCore.h"

import Core;
import Core.Platform.Windows;

WindowsIOCPImplementation::WindowsIOCPImplementation(size_t ThreadsHint)
{
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadsHint == -1 ? MAXDWORD : (DWORD)ThreadsHint);
	if (hIOCP == NULL)
	{
		throw SystemException(GetLastError());
	}
}

WindowsIOCPImplementation::~WindowsIOCPImplementation() noexcept
{
	if (hIOCP != NULL)
	{
		// IOCP handle need not to close.
		hIOCP = NULL;
	}
}

bool WindowsIOCPImplementation::poll_one()
{
	ULONG_PTR lpKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	DWORD dwTransferred = 0;
	BOOL bSucceeded = GetQueuedCompletionStatus(hIOCP, &dwTransferred, &lpKey, &lpOverlapped, INFINITE);

	if (lpOverlapped == NULL)
	{
		return false;
	}

	auto* pBlock = IOCompletionOverlapped::FromOverlapped(lpOverlapped);
	if (bSucceeded)
	{
		pBlock->Complete((size_t)dwTransferred);
	}
	else
	{
		pBlock->Failed(GetLastError());
	}

	delete pBlock;
	return true;
}

void WindowsIOCPImplementation::bind_handle(void* Handle)
{
	HANDLE hNewIOCP = CreateIoCompletionPort((HANDLE)Handle, hIOCP, (ULONG_PTR)Handle, 0);
	if (hNewIOCP == NULL)
	{
		throw SystemException(GetLastError());
	}
}

void WindowsIOCPImplementation::post(std::function<void(size_t, int32)> InWork)
{
	auto* ptr = new IOCompletionOverlapped(std::move(InWork));
	if (PostQueuedCompletionStatus(hIOCP, 0, (ULONG_PTR)ptr, reinterpret_cast<LPOVERLAPPED>(ptr->ToOverlapped())) == FALSE)
	{
		throw SystemException(GetLastError());
	}
}

#endif