// Copyright 2020-2025 Aumoa.lib. All right reserved.

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "Socket.Common.inl"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Threading/Tasks/TaskFactory.h"
#include "Net/Sockets/SocketException.h"
#include "IO/IOCompletionOverlapped.h"
#include <ws2tcpip.h>
#include <memory>

namespace Ayla
{
	struct Socket::PlatformSocket
	{
		using socket_len_type = int;

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
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
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
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
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
				ThreadPool::UnbindHandle((void*)m_Socket);
				closesocket(m_Socket);
			}
		}

		static void ThrowIfFailure(int resultCode)
		{
			if (resultCode == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
		}

		[[noreturn]]
		static void Throw()
		{
			throw SocketException(static_cast<SocketError>(WSAGetLastError()));
		}

		// Async operations - implementation using IOCP
		Task<std::shared_ptr<Socket>> AcceptAsync(std::stop_token cancellationToken)
		{
			SOCKET clientSock = WSASocketW(AddressFamilyToInt32(m_AddressFamily), SocketTypeToInt32(m_SocketType), 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
			char buffer[(sizeof(sockaddr_in) + 16) * 2]; // Buffer for local and remote addresses
			auto tcs = TaskCompletionSource<int32>::Create(cancellationToken);
			auto overlapped = new IOCompletionOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t, int32 status)
			{
				tcs.SetResult(status);
				delete overlapped;
			});

			int32 status = AcceptEx(m_Socket, clientSock, buffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, nullptr, (OVERLAPPED*)overlapped->ToOverlapped());
			if (status == SOCKET_ERROR && status != WSAEWOULDBLOCK)
			{
				closesocket(clientSock);
				throw SocketException(static_cast<SocketError>(status));
			}

			try
			{
				status = co_await tcs.GetTask();
				if (status != 0)
				{
					throw SocketException(static_cast<SocketError>(status));
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
			if (!m_IsBound)
			{
				sockaddr_in anyAddr = {};
				anyAddr.sin_family = AF_INET;
				anyAddr.sin_addr.s_addr = INADDR_ANY;
				anyAddr.sin_port = 0;
				bind(m_Socket, (sockaddr*)&anyAddr, sizeof(anyAddr));
				m_IsBound = true;
			}

			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);

			auto tcs = TaskCompletionSource<int32>::Create(cancellationToken);
			auto overlapped = new IOCompletionOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t, int32 status)
			{
				tcs.SetResult(status);
				delete overlapped;
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

			BOOL result = lpfnConnectEx(m_Socket, (sockaddr*)&addr, addrLen, NULL, NULL, NULL, (OVERLAPPED*)overlapped->ToOverlapped());
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					throw SocketException(static_cast<SocketError>(err));
				}

				result = (int)co_await tcs.GetTask();
				if (result != 0)
				{
					throw SocketException(static_cast<SocketError>(result));
				}
			}
		}

		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
		{
			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			std::vector<uint8> sendBuffer(buffer.begin(), buffer.end());
			auto overlapped = new IOCompletionOverlapped();
			auto* ovp = (OVERLAPPED*)overlapped->ToOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t sent, int32 status)
			{
				if (status != 0)
				{
					tcs.TrySetException(std::make_exception_ptr(SocketException(static_cast<SocketError>(status))));
				}
				else
				{
					tcs.SetResult(sent);
				}

				delete overlapped;
			});

			WSABUF buf
			{
				.len = (ULONG)buffer.size_bytes(),
				.buf = (CHAR*)buffer.data()
			};

			DWORD sent;
			BOOL result = WSASend(m_Socket, &buf, 1, &sent, 0, ovp, NULL);
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					delete overlapped;
					throw SocketException(static_cast<SocketError>(err));
				}

				sent = (DWORD)co_await tcs.GetTask();
			}

			co_return (size_t)sent;
		}

		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
		{
			WSABUF buf =
			{
				.len = (ULONG)buffer.size_bytes(),
				.buf = (CHAR*)buffer.data()
			};

			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto overlapped = new IOCompletionOverlapped();
			auto* ovp = (OVERLAPPED*)overlapped->ToOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t recv, int32 status)
			{
				if (status != 0)
				{
					tcs.TrySetException(std::make_exception_ptr(SocketException(static_cast<SocketError>(status))));
				}
				else
				{
					tcs.SetResult(recv);
				}

				delete overlapped;
			});

			DWORD received, flags = 0;
			BOOL result = WSARecv(m_Socket, &buf, 1, &received, &flags, ovp, NULL);
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					delete overlapped;
					throw SocketException(static_cast<SocketError>(err));
				}

				received = (DWORD)co_await tcs.GetTask();
			}

			co_return (size_t)received;
		}

		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);

			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto overlapped = new IOCompletionOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t sent, int32 status)
			{
				if (status != 0)
				{
					tcs.TrySetException(std::make_exception_ptr(SocketException(static_cast<SocketError>(status))));
				}
				else
				{
					tcs.SetResult(sent);
				}

				delete overlapped;
			});

			WSABUF buf
			{
				.len = (ULONG)buffer.size_bytes(),
				.buf = (CHAR*)buffer.data()
			};

			DWORD sent;
			BOOL result = WSASendTo(m_Socket, &buf, 1, &sent, 0, reinterpret_cast<sockaddr*>(&addr), addrLen, (OVERLAPPED*)overlapped->ToOverlapped(), NULL);
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					delete overlapped;
					throw SocketException(static_cast<SocketError>(err));
				}

				sent = (DWORD)co_await tcs.GetTask();
			}

			co_return (size_t)sent;
		}

		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			sockaddr_storage addr;
			int addrLen = sizeof(addr);

			WSABUF buf =
			{
				.len = (ULONG)buffer.size_bytes(),
				.buf = (CHAR*)buffer.data()
			};

			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto overlapped = new IOCompletionOverlapped();
			overlapped->SetOnCompletion([tcs, overlapped](size_t recv, int32 status)
			{
				if (status != 0)
				{
					tcs.TrySetException(std::make_exception_ptr(SocketException(static_cast<SocketError>(status))));
				}
				else
				{
					tcs.SetResult(recv);
				}

				delete overlapped;
			});

			DWORD received;
			DWORD flags = 0;
			BOOL result = WSARecvFrom(m_Socket, &buf, 1, &received, &flags, reinterpret_cast<sockaddr*>(&addr), &addrLen, (OVERLAPPED*)overlapped->ToOverlapped(), NULL);
			if (result == SOCKET_ERROR)
			{
				int err = WSAGetLastError();
				if (err != WSA_IO_PENDING)
				{
					delete overlapped;
					throw SocketException(static_cast<SocketError>(err));
				}

				received = (DWORD)co_await tcs.GetTask();
			}

			remoteEP = SockAddrToIPEndPoint(addr);
			co_return (size_t)received;
		}

		// Properties and utility methods
		bool IsConnected() const { return m_IsConnected; }
		bool IsBound() const { return m_IsBound; }
		bool IsListening() const { return m_IsListening; }
		AddressFamily GetAddressFamily() const { return m_AddressFamily; }
		SocketType GetSocketType() const { return m_SocketType; }

		void SetSocketOption(int32 level, int32 optionName, bool optionValue)
		{
			BOOL value = optionValue ? TRUE : FALSE;
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(&value), sizeof(value)) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
		}

		void SetSocketOption(int32 level, int32 optionName, int32 optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(&optionValue), sizeof(optionValue)) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
		}

		void SetSocketOption(int32 level, int32 optionName, std::span<const uint8> optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, reinterpret_cast<const char*>(optionValue.data()), 
				static_cast<int>(optionValue.size())) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
		}

		bool GetSocketOptionBool(int32 level, int32 optionName) const
		{
			BOOL value;
			int valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(&value), &valueLen) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
			return value != FALSE;
		}

		int32 GetSocketOptionInt32(int32 level, int32 optionName) const
		{
			int32 value;
			int valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(&value), &valueLen) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
			return value;
		}

		std::vector<uint8> GetSocketOptionBytes(int32 level, int32 optionName) const
		{
			std::vector<uint8> buffer(256);
			int bufferLen = static_cast<int>(buffer.size());

			if (getsockopt(m_Socket, level, optionName, reinterpret_cast<char*>(buffer.data()), &bufferLen) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}

			buffer.resize(bufferLen);
			return buffer;
		}

		void Shutdown(SocketShutdown how)
		{
			if (shutdown(m_Socket, (int)how) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
			}
		}

		int32 GetAvailable() const
		{
			u_long available;
			if (ioctlsocket(m_Socket, FIONREAD, &available) == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
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
				throw SocketException(static_cast<SocketError>(WSAEINVAL));
			}

			if (result == SOCKET_ERROR)
			{
				throw SocketException(static_cast<SocketError>(WSAGetLastError()));
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
	};
}

#undef __ALLOW_PLATFORM_COMMON_H__
