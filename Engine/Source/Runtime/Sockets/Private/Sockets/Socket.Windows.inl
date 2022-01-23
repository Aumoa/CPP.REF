// Copyright 2020-2021 Aumoa.lib. All right reserved.

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
	}

	~SocketImpl() noexcept
	{
		if (--ReferenceCount == 0)
		{
			std::unique_lock StaticMtx_lock(StaticMtx);
			WSACleanup();
		}
	}
};

#endif