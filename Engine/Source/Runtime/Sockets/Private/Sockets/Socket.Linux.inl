// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_LINUX

#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

constexpr int SOCKET_ERROR = -1;
constexpr int INVALID_SOCKET = -1;
using SOCKET = int;

using ReadbackBuf = void*;

#define ioctlsocket ioctl

struct SSocket::SocketImpl
{
	int Socket = -1;
	bool bClosed = false;

	void Close()
	{
		if (!bClosed)
		{
			if (close(Socket) == SOCKET_ERROR)
			{
				throw socket_exception("Failed to close socket handle.");
			}
			bClosed = true;
		}
	}

	[[noreturn]]
	void AbortWithError(const std::source_location& src = std::source_location::current())
	{
		throw socket_exception(String::Format(L"An exception occurred while processing socket. errno: {}", errno), src);
	}
};

#endif