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

		inline std::exception_ptr MakeSocketException(int32 error)
		{
			return std::make_exception_ptr(SocketException(static_cast<SocketError>(error)));
		}

		template<class T>
		void SetSocketException(TaskCompletionSource<T> tcs, int32 error)
		{
			tcs.TrySetException(MakeSocketException(error));
		}

		inline void SetSocketResult(TaskCompletionSource<> tcs)
		{
			if (!tcs.GetTask().IsCompleted())
			{
				tcs.SetResult();
			}
		}

		template<class T, class U>
		void SetSocketResult(TaskCompletionSource<T> tcs, U&& result)
		{
			if (!tcs.GetTask().IsCompleted())
			{
				tcs.SetResult(std::forward<U>(result));
			}
		}

		inline int32 GetSubmitFailureError() noexcept
		{
			return errno != 0 ? errno : EAGAIN;
		}
	}
}
