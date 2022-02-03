// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <WinSock2.h>
#undef Yield

struct SSocket::SocketImpl
{
	inline static volatile bool bStaticInit = false;
	inline static std::atomic<size_t> ReferenceCount;
	inline static WSADATA WSAData;
	inline static std::mutex StaticMtx;

	SOCKET Socket = 0;
	HANDLE hSockSendEvent = nullptr;
	HANDLE hSockRecvEvent = nullptr;

public:
	SocketImpl(EAddressFamily Family, ESocketType SocketType, EProtocolType ProtocolType)
	{
		[[unlikely]]
		if (!bStaticInit)
		{
			std::unique_lock StaticMtx_lock(StaticMtx);

			if (!bStaticInit)
			{
				// Initialize WinSock.
				int32 Result = WSAStartup(MAKEWORD(2, 2), &WSAData);
				if (Result)
				{
					throw gcnew SSocketException(std::format(L"Couldn't initialize WinSock2 with error code: {}", Result));
				}

				bStaticInit = true;
			}
		}

		++ReferenceCount;

		hSockSendEvent = WSACreateEvent();
		hSockRecvEvent = WSACreateEvent();
	}

	inline void CloseHandle(HANDLE& hSockEvent)
	{
		if (hSockEvent)
		{
			WSACloseEvent(hSockEvent);
			hSockEvent = nullptr;
		}
	}

	~SocketImpl() noexcept
	{
		CloseHandle(hSockSendEvent);
		CloseHandle(hSockRecvEvent);

		if (--ReferenceCount == 0)
		{
			std::unique_lock StaticMtx_lock(StaticMtx);
			WSACleanup();
		}
	}

	Task<> Connect(const IPEndPoint& EndPoint)
	{
		auto InAddr = GetSOCKADDR_IN(EndPoint);
		int32 SocketError = connect(Socket, (SOCKADDR*)&InAddr, sizeof(InAddr));
		if (SocketError == 0)
		{
			co_return;
		}

		int32 ConnectCode = WSAGetLastError();
		int32 CodeSize = sizeof(int32);

		if (ConnectCode == 0)
		{
			co_return;
		}
		else if (ConnectCode == WSAEWOULDBLOCK)
		{
			ConnectCode = co_await WaitSocketEvent(hSockSendEvent, FD_CONNECT_BIT, L"Connect()");
			switch (ConnectCode)
			{
			case 0:
				co_return;
			default:
				AbortWithError(ConnectCode, L"Connect()");
			}
		}

		AbortWithError(ConnectCode, L"Connect()");
	}

	Task<> Send(const void* Buf, size_t Size)
	{
		while (Size > 0)
		{
			int32 SendBuf = ClampToInt(Size);
			SendBuf = send(Socket, (const char*)Buf, SendBuf, 0);

			if (SendBuf == -1)
			{
				int32 Status = WSAGetLastError();
				if (Status == WSAEWOULDBLOCK)
				{
					Status = co_await WaitSocketEvent(hSockSendEvent, FD_WRITE_BIT, L"Send()");
					if (Status < 0)
					{
						AbortWithError(Status, L"Send()");
					}
					else
					{
						SendBuf = Status;
					}
				}
				else
				{
					AbortWithError(Status, L"Send()");
				}
			}

			Size -= (size_t)SendBuf;
		}
	}

	Task<size_t> Recv(void* OutBuf, size_t Size, bool bVerifiedLength)
	{
		size_t TotalSize = 0;

		while (Size > 0)
		{
			int32 RecvBuf = ClampToInt(Size);
			RecvBuf = recv(Socket, (char*)OutBuf, RecvBuf, 0);

			if (RecvBuf == -1)
			{
				int32 Status = WSAGetLastError();
				if (Status == WSAEWOULDBLOCK)
				{
					Status = co_await WaitSocketEvent(hSockRecvEvent, FD_READ_BIT, L"Recevie()");
					if (Status < 0)
					{
						AbortWithError(Status, L"Recevie()");
					}
					else
					{
						RecvBuf = Status;
					}
				}
				else
				{
					AbortWithError(Status, L"Send()");
				}
			}

			if (!bVerifiedLength && RecvBuf == 0)
			{
				break;
			}

			Size -= (size_t)RecvBuf;
			TotalSize += (size_t)RecvBuf;
		}

		co_return TotalSize;
	}

private:
	Task<int32> WaitSocketEvent(HANDLE& hSockEvent, int32 ConnectBit, std::wstring_view Op)
	{
		WSAResetEvent(hSockEvent);
		if (WSAEventSelect(Socket, hSockEvent, 1 << ConnectBit) == SOCKET_ERROR)
		{
			AbortWithError(WSAGetLastError(), Op);
		}

		while (true)
		{
			DWORD Status = WSAWaitForMultipleEvents(1, &hSockEvent, true, 32, false);
			if (Status == WSA_WAIT_FAILED)
			{
				AbortWithError(WSAGetLastError(), Op);
			}
			else if (Status == WSA_WAIT_TIMEOUT)
			{
				co_await Task<>::Yield();
			}
			else
			{
				break;
			}
		}

		WSANETWORKEVENTS Events;
		if (WSAEnumNetworkEvents(Socket, hSockEvent, &Events) == SOCKET_ERROR)
		{
			AbortWithError(WSAGetLastError(), Op);
		}

		co_return Events.iErrorCode[ConnectBit];
	}

	[[noreturn]]
	static void AbortWithError(int32 PlatformErrorCode, std::wstring_view Operation)
	{
		wchar_t* Wstr = nullptr;
		std::wstring Message;

		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			(DWORD)PlatformErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&Wstr,
			0,
			nullptr
		);

		if (Wstr)
		{
			Message = Wstr;
			LocalFree(Wstr);
			Wstr = nullptr;
		}

		throw gcnew SSocketException(std::format(L"{} operation return error code: {}, message: {}", Operation, PlatformErrorCode, Message));
	}

	inline SOCKADDR_IN GetSOCKADDR_IN(const IPEndPoint& EndPoint)
	{
		SOCKADDR_IN InAddr = {};
		InAddr.sin_family = AF_INET;
		InAddr.sin_addr.s_addr = EndPoint.IP.Address;
		InAddr.sin_port = EndPoint.GetNetPort();
		return InAddr;
	}

	inline int32 ClampToInt(size_t Size)
	{
		return (int32)(Size > 0x7FFFFFFF ? 0x7FFFFFFF : Size);
	}
};

#endif