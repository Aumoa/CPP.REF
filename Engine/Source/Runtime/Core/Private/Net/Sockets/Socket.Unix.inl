// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <memory>
#include <stop_token>
#include <tuple>
#include <utility>
#include <vector>
#include "Socket.Common.inl"
#include "Socket.Unix.AsyncCommon.inl"

#if PLATFORM_LINUX
#include "Socket.Unix.Async.Linux.inl"
#elif PLATFORM_OSX
#include "Socket.Unix.Async.OSX.inl"
#endif

namespace Ayla
{
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
			auto tcs = TaskCompletionSource<std::shared_ptr<Socket>>::Create(cancellationToken);
			std::ignore = UnixSocketAsyncBackend::AcceptAsync(m_Socket, cancellationToken)
				.ContinueWith([tcs](Task<int> task)
				{
					if (task.IsCanceled())
					{
						tcs.TrySetCanceled();
						return;
					}
					if (task.IsFaulted())
					{
						tcs.TrySetException(task.GetException());
						return;
					}

					int acceptedSocket = task.GetResult();
					SetSocketResult(tcs, Socket::CreateFromPlatformSocket(std::make_unique<PlatformSocket>(acceptedSocket)));
				});

			return tcs.GetTask();
		}

		Task<> ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			auto tcs = TaskCompletionSource<>::Create(cancellationToken);
			std::ignore = UnixSocketAsyncBackend::ConnectAsync(m_Socket, remoteEP, cancellationToken)
				.ContinueWith([this, tcs](Task<> task)
				{
					if (task.IsCanceled())
					{
						tcs.TrySetCanceled();
						return;
					}
					if (task.IsFaulted())
					{
						tcs.TrySetException(task.GetException());
						return;
					}

					task.GetResult();
					m_IsConnected = true;
					SetSocketResult(tcs);
				});

			return tcs.GetTask();
		}

		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
		{
			return UnixSocketAsyncBackend::SendAsync(m_Socket, buffer, cancellationToken);
		}

		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
		{
			return UnixSocketAsyncBackend::ReceiveAsync(m_Socket, buffer, cancellationToken);
		}

		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			return UnixSocketAsyncBackend::SendToAsync(m_Socket, buffer, remoteEP, cancellationToken);
		}

		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			std::ignore = UnixSocketAsyncBackend::ReceiveFromAsync(m_Socket, buffer, cancellationToken)
				.ContinueWith([tcs, &remoteEP](Task<UnixSocketReceiveFromResult> task)
				{
					if (task.IsCanceled())
					{
						tcs.TrySetCanceled();
						return;
					}
					if (task.IsFaulted())
					{
						tcs.TrySetException(task.GetException());
						return;
					}

					UnixSocketReceiveFromResult result = task.GetResult();
					remoteEP = result.m_RemoteEndPoint;
					SetSocketResult(tcs, result.m_BytesTransferred);
				});

			return tcs.GetTask();
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
