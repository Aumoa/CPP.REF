// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IntegralTypes.h"
#include "Net/IPEndPoint.h"
#include "Net/Sockets/SocketError.h"
#include "Net/Sockets/SocketException.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include <cerrno>
#include <cstddef>
#include <exception>
#include <utility>

namespace Ayla
{
	inline constexpr int INVALID_SOCKET = -1;
	inline constexpr int SOCKET_ERROR = -1;

	namespace
	{
		struct UnixSocketReceiveFromResult
		{
			size_t m_BytesTransferred;
			IPEndPoint m_RemoteEndPoint;
		};

		inline SocketError UnixErrnoToSocketError(int32 error) noexcept
		{
			if (error == 0)
			{
				return SocketError::Success;
			}

#ifdef EINTR
			if (error == EINTR)
			{
				return SocketError::Interrupted;
			}
#endif
#if defined(EACCES) || defined(EPERM)
			if (
#ifdef EACCES
				error == EACCES
#endif
#if defined(EACCES) && defined(EPERM)
				||
#endif
#ifdef EPERM
				error == EPERM
#endif
			)
			{
				return SocketError::AccessDenied;
			}
#endif
#ifdef EFAULT
			if (error == EFAULT)
			{
				return SocketError::Fault;
			}
#endif
#ifdef EINVAL
			if (error == EINVAL)
			{
				return SocketError::InvalidArgument;
			}
#endif
#if defined(EAGAIN) || defined(EWOULDBLOCK)
			if (
#ifdef EAGAIN
				error == EAGAIN
#endif
#if defined(EAGAIN) && defined(EWOULDBLOCK)
				||
#endif
#ifdef EWOULDBLOCK
				error == EWOULDBLOCK
#endif
			)
			{
				return SocketError::WouldBlock;
			}
#endif
#ifdef EINPROGRESS
			if (error == EINPROGRESS)
			{
				return SocketError::InProgress;
			}
#endif
#ifdef EALREADY
			if (error == EALREADY)
			{
				return SocketError::AlreadyInProgress;
			}
#endif
#ifdef ENOTSOCK
			if (error == ENOTSOCK)
			{
				return SocketError::NotSocket;
			}
#endif
#ifdef EMSGSIZE
			if (error == EMSGSIZE)
			{
				return SocketError::MessageSize;
			}
#endif
#ifdef EPROTONOSUPPORT
			if (error == EPROTONOSUPPORT)
			{
				return SocketError::ProtocolNotSupported;
			}
#endif
#ifdef ESOCKTNOSUPPORT
			if (error == ESOCKTNOSUPPORT)
			{
				return SocketError::SocketTypeNotSupported;
			}
#endif
#if defined(EOPNOTSUPP) || defined(ENOTSUP)
			if (
#ifdef EOPNOTSUPP
				error == EOPNOTSUPP
#endif
#if defined(EOPNOTSUPP) && defined(ENOTSUP)
				||
#endif
#ifdef ENOTSUP
				error == ENOTSUP
#endif
			)
			{
				return SocketError::OperationNotSupported;
			}
#endif
#ifdef EAFNOSUPPORT
			if (error == EAFNOSUPPORT)
			{
				return SocketError::AddressFamilyNotSupported;
			}
#endif
#ifdef EADDRINUSE
			if (error == EADDRINUSE)
			{
				return SocketError::AddressAlreadyInUse;
			}
#endif
#ifdef EADDRNOTAVAIL
			if (error == EADDRNOTAVAIL)
			{
				return SocketError::AddressNotAvailable;
			}
#endif
#ifdef ENETDOWN
			if (error == ENETDOWN)
			{
				return SocketError::NetworkDown;
			}
#endif
#ifdef ENETUNREACH
			if (error == ENETUNREACH)
			{
				return SocketError::NetworkUnreachable;
			}
#endif
#ifdef ENETRESET
			if (error == ENETRESET)
			{
				return SocketError::NetworkReset;
			}
#endif
#ifdef ECONNABORTED
			if (error == ECONNABORTED)
			{
				return SocketError::ConnectionAborted;
			}
#endif
#ifdef ECONNRESET
			if (error == ECONNRESET)
			{
				return SocketError::ConnectionReset;
			}
#endif
#ifdef EPIPE
			if (error == EPIPE)
			{
				return SocketError::ConnectionReset;
			}
#endif
#if defined(ENOBUFS) || defined(ENOMEM)
			if (
#ifdef ENOBUFS
				error == ENOBUFS
#endif
#if defined(ENOBUFS) && defined(ENOMEM)
				||
#endif
#ifdef ENOMEM
				error == ENOMEM
#endif
			)
			{
				return SocketError::NoBufferSpaceAvailable;
			}
#endif
#ifdef EISCONN
			if (error == EISCONN)
			{
				return SocketError::AlreadyConnected;
			}
#endif
#ifdef ENOTCONN
			if (error == ENOTCONN)
			{
				return SocketError::NotConnected;
			}
#endif
#ifdef ESHUTDOWN
			if (error == ESHUTDOWN)
			{
				return SocketError::Shutdown;
			}
#endif
#ifdef ETIMEDOUT
			if (error == ETIMEDOUT)
			{
				return SocketError::TimedOut;
			}
#endif
#ifdef ECONNREFUSED
			if (error == ECONNREFUSED)
			{
				return SocketError::ConnectionRefused;
			}
#endif
#ifdef EHOSTDOWN
			if (error == EHOSTDOWN)
			{
				return SocketError::HostDown;
			}
#endif
#ifdef EHOSTUNREACH
			if (error == EHOSTUNREACH)
			{
				return SocketError::HostUnreachable;
			}
#endif
#if defined(EMFILE) || defined(ENFILE)
			if (
#ifdef EMFILE
				error == EMFILE
#endif
#if defined(EMFILE) && defined(ENFILE)
				||
#endif
#ifdef ENFILE
				error == ENFILE
#endif
			)
			{
				return SocketError::ProcessLimit;
			}
#endif
#ifdef ECANCELED
			if (error == ECANCELED)
			{
				return SocketError::OperationAborted;
			}
#endif
#ifdef ENOSYS
			if (error == ENOSYS)
			{
				return SocketError::NotSupported;
			}
#endif

			return SocketError::SocketError;
		}

		inline std::exception_ptr MakeSocketException(int32 error)
		{
			return std::make_exception_ptr(SocketException(UnixErrnoToSocketError(error)));
		}

		template<class T>
		void SetSocketException(TaskCompletionSource<T> tcs, int32 error)
		{
			tcs.TrySetException(MakeSocketException(error));
		}

		inline void SetSocketResult(TaskCompletionSource<> tcs)
		{
			tcs.TrySetResult();
		}

		template<class T, class U>
		void SetSocketResult(TaskCompletionSource<T> tcs, U&& result)
		{
			tcs.TrySetResult(std::forward<U>(result));
		}

		inline int32 GetSubmitFailureError() noexcept
		{
			return errno != 0 ? errno : EAGAIN;
		}
	}
}
