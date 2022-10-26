// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Windows/WindowsIOCP.h"
#include "Windows/WinSocket.h"
#include "Threading/ThreadGroup.h"
#include "Net/SocketException.h"
#include "WindowsIOCP.gen.cpp"

class IOCPThreadGroup : public ThreadGroup
{
public:
	IOCPThreadGroup(std::function<void()> invoke)
		: ThreadGroup(TEXT("IOCP"), 0, false, std::bind(&IOCPThreadGroup::ThreadStart, this, std::placeholders::_1, std::placeholders::_2, invoke))
	{
	}

	void ThreadStart(size_t index, std::stop_token cancellationToken, std::function<void()> invoke)
	{
		ReadyWorkerThread(index);
		invoke();
	}
};

WindowsIOCP::WindowsIOCP()
{
	// Initialize WSA.
	if (WSAStartup(MAKEWORD(2, 2), &_data) != 0)
	{
		check(false);
	}

	// Initialize completion port provider.
	_hComp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	_threads = std::make_unique<IOCPThreadGroup>(std::bind(&WindowsIOCP::IOCPThreadStart, this));
}

WindowsIOCP::~WindowsIOCP() noexcept
{
	if (_hComp)
	{
		CloseHandle(_hComp);
		_hComp = NULL;
	}
}

void WindowsIOCP::HandleChainSocket(ImplSocket* impl)
{
	auto hSock = reinterpret_cast<SOCKET>(impl->GetSocket());

	// Connect socket chain to completion port
	// for accept event notifications.
	HANDLE hComp = CreateIoCompletionPort((HANDLE)hSock, _hComp, (ULONG_PTR)impl, 0);
	if (hComp == NULL)
	{
		throw SocketException(FormatLastError());
	}

	// Swap handle only completed.
	_hComp = hComp;
}

void WindowsIOCP::IOCPThreadStart()
{
	while (true)
	{
		try
		{
			while (true)
			{
				DWORD dwTransferred = 0;
				ULONG_PTR lpKey = 0;
				OVERLAPPED* lpOverlapped = nullptr;
				if (GetQueuedCompletionStatus(_hComp, &dwTransferred, &lpKey, &lpOverlapped, 1000) == FALSE)
				{
					continue;
				}

				auto* pWinOverlapped = static_cast<WinOverlapped*>(lpOverlapped);
				auto signal = pWinOverlapped->Signal;
				pWinOverlapped->Release();
				signal.SetResult((size_t)dwTransferred);
			}
		}
		catch (...)
		{
			// Uncaught error detected!
			check(false);
		}
	}
}

#endif