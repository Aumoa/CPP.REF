// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "Socket.Common.inl"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Threading/Tasks/TaskFactory.h"
#include "SystemException.h"
#include "IO/IOCompletionOverlapped.h"
#include <ws2tcpip.h>
#include <memory>

namespace Ayla
{
	struct Socket::PlatformSocket
	{
		const SOCKET m_Socket;
		const AddressFamily m_AddressFamily;
		const SocketType m_SocketType;
		bool m_IsBound = false;
		bool m_IsListening = false;
		bool m_IsConnected = false;

		static void Initialize()
		{
			static WSAData wsa;
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			{
				throw SystemException(WSAGetLastError(), TEXT("Failed to initialize Winsock"));
			}
		}

		PlatformSocket(AddressFamily af, SocketType st)
			: m_Socket{ WSASocketW(AddressFamilyToInt32(af), SocketTypeToInt32(st), 0, NULL, 0, WSA_FLAG_OVERLAPPED) }
			, m_AddressFamily(af)
			, m_SocketType(st)
		{
			ThreadPool::BindHandle((void*)m_Socket);

			if (m_Socket == INVALID_SOCKET)
			{
				throw SystemException(WSAGetLastError(), TEXT("Failed to create socket"));
			}
		}

		static std::tuple<AddressFamily, SocketType> GetOptions(SOCKET socket)
		{
			WSAPROTOCOL_INFO protoInfo;
			int len = sizeof(protoInfo);

			int result = getsockopt(
				socket,
				SOL_SOCKET,
				SO_PROTOCOL_INFOW,
				(char*)&protoInfo,
				&len
			);

			check(result == 0);
			return std::make_pair(
				Int32ToAddressFamily(protoInfo.iAddressFamily),
				Int32ToSocketType(protoInfo.iSocketType)
			);
		}

		PlatformSocket(SOCKET socket, std::tuple<AddressFamily, SocketType> options)
			: m_Socket(socket)
			, m_AddressFamily(std::get<0>(options))
			, m_SocketType(std::get<1>(options))
			, m_IsConnected(true)
		{
		}

		explicit PlatformSocket(SOCKET socket)
			: PlatformSocket(socket, GetOptions(socket))
		{
		}

		~PlatformSocket() noexcept
		{
			if (m_Socket != INVALID_SOCKET)
			{
				closesocket(m_Socket);
			}
		}

		void Bind(const IPEndPoint& localEP)
		{
			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(localEP, addr);
			
			if (bind(m_Socket, reinterpret_cast<sockaddr*>(&addr), addrLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("Bind failed"));
			}
			m_IsBound = true;
		}

		void Listen(int32 backlog)
		{
			if (listen(m_Socket, backlog) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("Listen failed"));
			}
			m_IsListening = true;
		}

		void Connect(const IPEndPoint& remoteEP)
		{
			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);
			
			if (connect(m_Socket, reinterpret_cast<sockaddr*>(&addr), addrLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("Connect failed"));
			}
			m_IsConnected = true;
		}

		std::unique_ptr<PlatformSocket> Accept()
		{
			sockaddr_storage clientAddr;
			int clientAddrLen = sizeof(clientAddr);
			
			SOCKET clientSocket = accept(m_Socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
			if (clientSocket == INVALID_SOCKET)
			{
				throw SystemException(WSAGetLastError(), TEXT("Accept failed"));
			}

			return std::make_unique<PlatformSocket>(clientSocket);
		}

		size_t Send(std::span<const uint8> buffer)
		{
			int result = send(m_Socket, reinterpret_cast<const char*>(buffer.data()), 
				static_cast<int>(buffer.size()), 0);
			if (result == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("Send failed"));
			}
			return static_cast<size_t>(result);
		}

		size_t Receive(std::span<uint8> buffer)
		{
			int result = recv(m_Socket, reinterpret_cast<char*>(buffer.data()), 
				static_cast<int>(buffer.size()), 0);
			if (result == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("Receive failed"));
			}
			return static_cast<size_t>(result);
		}

		size_t SendTo(std::span<const uint8> buffer, const IPEndPoint& remoteEP)
		{
			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);
			
			int result = sendto(m_Socket, reinterpret_cast<const char*>(buffer.data()), 
				static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&addr), addrLen);
			if (result == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("SendTo failed"));
			}
			return static_cast<size_t>(result);
		}

		size_t ReceiveFrom(std::span<uint8> buffer, IPEndPoint& remoteEP)
		{
			sockaddr_storage addr;
			int addrLen = sizeof(addr);
			
			int result = recvfrom(m_Socket, reinterpret_cast<char*>(buffer.data()), 
				static_cast<int>(buffer.size()), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);
			if (result == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("ReceiveFrom failed"));
			}

			remoteEP = SockAddrToIPEndPoint(addr);
			return static_cast<size_t>(result);
		}

		// Async operations - implementation using IOCP
		Task<std::unique_ptr<Socket>> AcceptAsync(std::stop_token cancellationToken)
		{
			SOCKET clientSock = WSASocketW(AddressFamilyToInt32(m_AddressFamily), SocketTypeToInt32(m_SocketType), 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
			char buffer[(sizeof(sockaddr_in) + 16) * 2]; // Buffer for local and remote addresses
			auto tcs = TaskCompletionSource<int32>::Create(cancellationToken);
			auto overlapped = std::make_unique<IOCompletionOverlapped>([&tcs](IOCompletionOverlapped*, size_t, int32 status)
			{
				tcs.SetResult(status);
			});

			AcceptEx(m_Socket, clientSock, buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, nullptr, (OVERLAPPED*)overlapped->ToOverlapped());
			try
			{
				int32 status = co_await tcs.GetTask();
				if (status != 0)
				{
					throw SystemException(status, TEXT("AcceptEx failed"));
				}
			}
			catch (...)
			{
				closesocket(clientSock);
				throw;
			}

			// Configure socket.
			setsockopt(clientSock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_Socket, sizeof(m_Socket));
			ThreadPool::BindHandle((void*)clientSock);

			auto platformSocket = std::make_unique<PlatformSocket>(clientSock);
			co_return CreateFromPlatformSocket(std::move(platformSocket));
		}

		Task<> ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);

			auto tcs = TaskCompletionSource<int32>::Create(cancellationToken);
			auto overlapped = std::make_unique<IOCompletionOverlapped>([&tcs](IOCompletionOverlapped*, size_t, int32 status)
			{
				tcs.SetResult(status);
			});

			LPFN_CONNECTEX lpfnConnectEx = NULL;

			switch (m_SocketType)
			{
				case SocketType::Stream:
					// For stream sockets, we need to use ConnectEx for async connect.
					static LPFN_CONNECTEX lpfnConnectEx_Stream = [this]()
					{
						LPFN_CONNECTEX lpfnConnectEx = NULL;
						GUID guidConnectEx = WSAID_CONNECTEX;
						DWORD bytes;
						WSAIoctl(m_Socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
							&guidConnectEx, sizeof(guidConnectEx),
							&lpfnConnectEx, sizeof(lpfnConnectEx),
							&bytes, NULL, NULL);
						return lpfnConnectEx;
					}();
					lpfnConnectEx = lpfnConnectEx_Stream;
					break;
			}

			int result = lpfnConnectEx(m_Socket, (sockaddr*)&addr, addrLen, NULL, NULL, NULL, (OVERLAPPED*)overlapped->ToOverlapped());
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					throw SystemException(err, TEXT("WSAConnect failed"));
				}

				result = (int)co_await tcs.GetTask();
				if (result != 0)
				{
					throw SystemException(result, TEXT("WSAConnect failed"));
				}
			}
		}

		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
		{
			return Task<>::Run([this, buffer]() -> size_t
			{
				return Send(buffer);
			}, cancellationToken);
		}

		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
		{
			return Task<>::Run([this, buffer]() -> size_t
			{
				return Receive(buffer);
			}, cancellationToken);
		}

		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			return Task<>::Run([this, buffer, remoteEP]() -> size_t
			{
				return SendTo(buffer, remoteEP);
			}, cancellationToken);
		}

		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			return Task<>::Run([this, buffer, &remoteEP]() -> size_t
			{
				return ReceiveFrom(buffer, remoteEP);
			}, cancellationToken);
		}

		// Properties and utility methods
		bool IsConnected() const { return m_IsConnected; }
		bool IsBound() const { return m_IsBound; }
		bool IsListening() const { return m_IsListening; }
		AddressFamily GetAddressFamily() const { return m_AddressFamily; }
		SocketType GetSocketType() const { return m_SocketType; }

		IPEndPoint GetLocalEndPoint() const
		{
			sockaddr_storage addr;
			int addrLen = sizeof(addr);
			if (getsockname(m_Socket, reinterpret_cast<sockaddr*>(&addr), &addrLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("getsockname failed"));
			}
			return SockAddrToIPEndPoint(addr);
		}

		IPEndPoint GetRemoteEndPoint() const
		{
			sockaddr_storage addr;
			int addrLen = sizeof(addr);
			if (getpeername(m_Socket, reinterpret_cast<sockaddr*>(&addr), &addrLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("getpeername failed"));
			}
			return SockAddrToIPEndPoint(addr);
		}

		void SetSocketOption(int32 level, int32 optionName, bool optionValue)
		{
			BOOL value = optionValue ? TRUE : FALSE;
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(&value), sizeof(value)) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("setsockopt failed"));
			}
		}

		void SetSocketOption(int32 level, int32 optionName, int32 optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(&optionValue), sizeof(optionValue)) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("setsockopt failed"));
			}
		}

		void SetSocketOption(int32 level, int32 optionName, std::span<const uint8> optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(optionValue.data()), 
				static_cast<int>(optionValue.size())) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("setsockopt failed"));
			}
		}

		bool GetSocketOptionBool(int32 level, int32 optionName) const
		{
			BOOL value;
			int valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(&value), &valueLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("getsockopt failed"));
			}
			return value != FALSE;
		}

		int32 GetSocketOptionInt32(int32 level, int32 optionName) const
		{
			int32 value;
			int valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(&value), &valueLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("getsockopt failed"));
			}
			return value;
		}

		std::vector<uint8> GetSocketOptionBytes(int32 level, int32 optionName) const
		{
			std::vector<uint8> buffer(256);
			int bufferLen = static_cast<int>(buffer.size());
			
			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(buffer.data()), &bufferLen) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("getsockopt failed"));
			}
			
			buffer.resize(bufferLen);
			return buffer;
		}

		void Shutdown(int32 how)
		{
			if (shutdown(m_Socket, how) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("shutdown failed"));
			}
		}

		int32 GetAvailable() const
		{
			u_long available;
			if (ioctlsocket(m_Socket, FIONREAD, &available) == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("ioctlsocket FIONREAD failed"));
			}
			return static_cast<int32>(available);
		}

		bool Poll(int32 microSeconds, int32 mode) const
		{
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(m_Socket, &fds);

			timeval timeout;
			timeout.tv_sec = microSeconds / 1000000;
			timeout.tv_usec = microSeconds % 1000000;

			int result;
			switch (mode)
			{
			case 0: // SelectRead
				result = select(0, &fds, nullptr, nullptr, &timeout);
				break;
			case 1: // SelectWrite
				result = select(0, nullptr, &fds, nullptr, &timeout);
				break;
			case 2: // SelectError
				result = select(0, nullptr, nullptr, &fds, &timeout);
				break;
			default:
				throw SystemException(WSAEINVAL, TEXT("Invalid poll mode"));
			}

			if (result == SOCKET_ERROR)
			{
				throw SystemException(WSAGetLastError(), TEXT("select failed"));
			}

			return result > 0;
		}

	private:
		static int32 IPEndPointToSockAddr(const IPEndPoint& endPoint, sockaddr_storage& addr)
		{
			memset(&addr, 0, sizeof(addr));
			
			if (endPoint.GetAddressFamily() == AddressFamily::InterNetwork)
			{
				auto* addr4 = reinterpret_cast<sockaddr_in*>(&addr);
				addr4->sin_family = AF_INET;
				addr4->sin_port = htons(endPoint.GetPort());
				
				auto bytes = endPoint.GetAddress().GetAddressBytes();
				memcpy(&addr4->sin_addr, bytes.data(), 4);
				
				return sizeof(sockaddr_in);
			}
			else
			{
				auto* addr6 = reinterpret_cast<sockaddr_in6*>(&addr);
				addr6->sin6_family = AF_INET6;
				addr6->sin6_port = htons(endPoint.GetPort());
				
				auto bytes = endPoint.GetAddress().GetAddressBytes();
				memcpy(&addr6->sin6_addr, bytes.data(), 16);
				
				return sizeof(sockaddr_in6);
			}
		}

		static IPEndPoint SockAddrToIPEndPoint(const sockaddr_storage& addr)
		{
			if (addr.ss_family == AF_INET)
			{
				const auto* addr4 = reinterpret_cast<const sockaddr_in*>(&addr);
				std::array<uint8, 4> bytes;
				memcpy(bytes.data(), &addr4->sin_addr, 4);
				return IPEndPoint(IPAddress(bytes), ntohs(addr4->sin_port));
			}
			else if (addr.ss_family == AF_INET6)
			{
				const auto* addr6 = reinterpret_cast<const sockaddr_in6*>(&addr);
				std::array<uint8, 16> bytes;
				memcpy(bytes.data(), &addr6->sin6_addr, 16);
				return IPEndPoint(IPAddress(bytes), ntohs(addr6->sin6_port));
			}
			else
			{
				throw SystemException(WSAEAFNOSUPPORT, TEXT("Unsupported address family"));
			}
		}
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__