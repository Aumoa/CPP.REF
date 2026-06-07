// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <tuple>
#include <vector>
#include "Socket.Common.inl"

namespace Ayla
{
	inline constexpr int INVALID_SOCKET = -1;
	inline constexpr int SOCKET_ERROR = -1;

	struct Socket::PlatformSocket
	{
		using socket_len_type = socklen_t;

		const int m_Socket;
		const AddressFamily m_AddressFamily;
		const SocketType m_SocketType;
		bool m_IsBound = false;
		bool m_IsListening = false;
		bool m_IsConnected = false;

		static void Initialize()
		{
		}

		PlatformSocket(AddressFamily af, SocketType st)
			: m_Socket{ socket(AddressFamilyToInt32(af), SocketTypeToInt32(st), 0) }
			, m_AddressFamily(af)
			, m_SocketType(st)
		{
			if (m_Socket == INVALID_SOCKET)
			{
				Throw();
			}
		}

		static std::tuple<AddressFamily, SocketType> GetOptions(int socket)
		{
			sockaddr_storage addr;
			socket_len_type addrLen = sizeof(addr);
			if (getsockname(socket, reinterpret_cast<sockaddr*>(&addr), &addrLen) == SOCKET_ERROR)
			{
				Throw();
			}

			int socketType;
			socket_len_type socketTypeLen = sizeof(socketType);
			if (getsockopt(socket, SOL_SOCKET, SO_TYPE, &socketType, &socketTypeLen) == SOCKET_ERROR)
			{
				Throw();
			}

			return std::make_tuple(Int32ToAddressFamily(addr.ss_family), Int32ToSocketType(socketType));
		}

		PlatformSocket(int socket, std::tuple<AddressFamily, SocketType> options)
			: m_Socket(socket)
			, m_AddressFamily(std::get<0>(options))
			, m_SocketType(std::get<1>(options))
			, m_IsConnected(true)
		{
		}

		explicit PlatformSocket(int socket)
			: PlatformSocket(socket, GetOptions(socket))
		{
		}

		~PlatformSocket() noexcept
		{
			if (m_Socket != INVALID_SOCKET)
			{
				close(m_Socket);
			}
		}

		template<class TResult>
		static void ThrowIfFailure(TResult resultCode)
		{
			if (resultCode == static_cast<TResult>(SOCKET_ERROR))
			{
				Throw();
			}
		}

		[[noreturn]]
		static void Throw()
		{
			throw SocketException(static_cast<SocketError>(errno));
		}

		Task<std::shared_ptr<Socket>> AcceptAsync(std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			sockaddr_storage clientAddr;
			socket_len_type clientAddrLen = sizeof(clientAddr);
			auto clientSocket = accept(m_Socket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
			if (clientSocket == INVALID_SOCKET)
			{
				Throw();
			}

			co_return CreateFromPlatformSocket(std::make_unique<PlatformSocket>(clientSocket));
		}

		Task<> ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);
			int result = connect(m_Socket, reinterpret_cast<sockaddr*>(&addr), addrLen);
			ThrowIfFailure(result);
			m_IsConnected = true;
			co_return;
		}

		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			auto result = send(m_Socket, buffer.data(), buffer.size_bytes(), 0);
			ThrowIfFailure(result);
			co_return static_cast<size_t>(result);
		}

		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			auto result = recv(m_Socket, buffer.data(), buffer.size_bytes(), 0);
			ThrowIfFailure(result);
			co_return static_cast<size_t>(result);
		}

		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			sockaddr_storage addr;
			int addrLen = IPEndPointToSockAddr(remoteEP, addr);
			auto result = sendto(m_Socket, buffer.data(), buffer.size_bytes(), 0, reinterpret_cast<sockaddr*>(&addr), addrLen);
			ThrowIfFailure(result);
			co_return static_cast<size_t>(result);
		}

		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			(void)cancellationToken;

			sockaddr_storage addr;
			socket_len_type addrLen = sizeof(addr);
			auto result = recvfrom(m_Socket, buffer.data(), buffer.size_bytes(), 0, reinterpret_cast<sockaddr*>(&addr), &addrLen);
			ThrowIfFailure(result);
			remoteEP = SockAddrToIPEndPoint(addr);
			co_return static_cast<size_t>(result);
		}

		bool IsConnected() const { return m_IsConnected; }
		bool IsBound() const { return m_IsBound; }
		bool IsListening() const { return m_IsListening; }
		AddressFamily GetAddressFamily() const { return m_AddressFamily; }
		SocketType GetSocketType() const { return m_SocketType; }

		void SetSocketOption(int32 level, int32 optionName, bool optionValue)
		{
			int value = optionValue ? 1 : 0;
			if (setsockopt(m_Socket, level, optionName, &value, sizeof(value)) == SOCKET_ERROR)
			{
				Throw();
			}
		}

		void SetSocketOption(int32 level, int32 optionName, int32 optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, &optionValue, sizeof(optionValue)) == SOCKET_ERROR)
			{
				Throw();
			}
		}

		void SetSocketOption(int32 level, int32 optionName, std::span<const uint8> optionValue)
		{
			if (setsockopt(m_Socket, level, optionName, optionValue.data(), static_cast<socket_len_type>(optionValue.size_bytes())) == SOCKET_ERROR)
			{
				Throw();
			}
		}

		bool GetSocketOptionBool(int32 level, int32 optionName) const
		{
			int value;
			socket_len_type valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, &value, &valueLen) == SOCKET_ERROR)
			{
				Throw();
			}
			return value != 0;
		}

		int32 GetSocketOptionInt32(int32 level, int32 optionName) const
		{
			int32 value;
			socket_len_type valueLen = sizeof(value);
			if (getsockopt(m_Socket, level, optionName, &value, &valueLen) == SOCKET_ERROR)
			{
				Throw();
			}
			return value;
		}

		std::vector<uint8> GetSocketOptionBytes(int32 level, int32 optionName) const
		{
			std::vector<uint8> buffer(256);
			socket_len_type bufferLen = static_cast<socket_len_type>(buffer.size());

			if (getsockopt(m_Socket, level, optionName, buffer.data(), &bufferLen) == SOCKET_ERROR)
			{
				Throw();
			}

			buffer.resize(bufferLen);
			return buffer;
		}

		void Shutdown(SocketShutdown how)
		{
			if (shutdown(m_Socket, static_cast<int>(how)) == SOCKET_ERROR)
			{
				Throw();
			}
		}

		int32 GetAvailable() const
		{
			int available;
			if (ioctl(m_Socket, FIONREAD, &available) == SOCKET_ERROR)
			{
				Throw();
			}
			return available;
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
			case 0:
				result = select(m_Socket + 1, &fds, nullptr, nullptr, &timeout);
				break;
			case 1:
				result = select(m_Socket + 1, nullptr, &fds, nullptr, &timeout);
				break;
			case 2:
				result = select(m_Socket + 1, nullptr, nullptr, &fds, &timeout);
				break;
			default:
				throw SocketException(SocketError::InvalidArgument);
			}

			ThrowIfFailure(result);
			return result > 0;
		}
	};
}
