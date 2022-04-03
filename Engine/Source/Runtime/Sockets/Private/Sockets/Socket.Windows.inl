// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include <Windows.h>
#include <WinSock2.h>
#undef Yield

using socklen_t = int;
using ReadbackBuf = char*;

struct SSocket::SocketImpl
{
	inline static volatile bool bStaticInit = false;
	inline static std::atomic<size_t> ReferenceCount;
	inline static WSADATA WSAData;
	inline static std::mutex StaticMtx;

	SOCKET Socket = INVALID_SOCKET;
	HANDLE hSockListenEvent = nullptr;
	HANDLE hSockSendEvent = nullptr;
	HANDLE hSockRecvEvent = nullptr;
	bool bClosed = false;

public:
	SocketImpl()
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
					throw socket_exception(std::format("Couldn't initialize WinSock2 with error code: {}", Result));
				}

				bStaticInit = true;
			}
		}

		++ReferenceCount;

		hSockListenEvent = WSACreateEvent();
		hSockSendEvent = WSACreateEvent();
		hSockRecvEvent = WSACreateEvent();

		RegisterWaitForSingleObject(&hSockListenEvent, hSockListenEvent, SSocket::SocketImpl::WindowsAccept, this, 0, 0);
	}

	inline static void CloseHandle(HANDLE& hSockEvent)
	{
		if (hSockEvent)
		{
			WSACloseEvent(hSockEvent);
			hSockEvent = nullptr;
		}
	}

	~SocketImpl() noexcept
	{
		CloseHandle(hSockListenEvent);
		CloseHandle(hSockSendEvent);
		CloseHandle(hSockRecvEvent);

		if (--ReferenceCount == 0)
		{
			std::unique_lock StaticMtx_lock(StaticMtx);
			WSACleanup();
		}
	}

	void SetNonblock()
	{
		u_long nBlock = 1;
		if (ioctlsocket(Socket, FIONBIO, &nBlock) == SOCKET_ERROR)
		{
			AbortWithError();
		}
	}

	void Close()
	{
		if (!bClosed)
		{
			if (closesocket(Socket) == SOCKET_ERROR)
			{
				throw socket_exception("Failed to close socket handle.");
			}
			bClosed = true;
		}
	}

	[[noreturn]]
	static void AbortWithError(const std::source_location& src = std::source_location::current())
	{
		int32 PlatformErrorCode = WSAGetLastError();
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

		throw socket_exception(std::format(L"Socket operation return error code: {}, message: {}", PlatformErrorCode, Message), src);
	}

	inline int32 ClampToInt(size_t Size)
	{
		return (int32)(Size > 0x7FFFFFFF ? 0x7FFFFFFF : Size);
	}

private:
	static VOID NTAPI WindowsAccept(PVOID ptr, BOOLEAN timerOrWaitFired)
	{
		SE_LOG(LogTemp, Verbose, L"WindowsAccept");
	}
};

#endif