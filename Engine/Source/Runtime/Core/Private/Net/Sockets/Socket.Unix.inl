// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <exception>
#include <memory>
#include <mutex>
#include <stop_token>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>
#include "MoveOnlyFunction.h"
#include "Threading/ThreadPool.h"
#include "Threading/Tasks/TaskCompletionSource.h"
#include "Socket.Common.inl"

#if PLATFORM_LINUX
#include <liburing.h>
#elif PLATFORM_OSX
#include <sys/event.h>
#endif

namespace Ayla
{
	inline constexpr int INVALID_SOCKET = -1;
	inline constexpr int SOCKET_ERROR = -1;

	namespace
	{
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

#if PLATFORM_LINUX
		class LinuxSocketOperation
		{
			MoveOnlyFunction<void(int32)> m_Completion;

		public:
			explicit LinuxSocketOperation(MoveOnlyFunction<void(int32)> completion)
				: m_Completion(std::move(completion))
			{
			}

			void Complete(int32 result)
			{
				m_Completion(result);
			}
		};

		class LinuxSocketAsyncQueue
		{
			static constexpr uint32 kQueueDepth = 256;

			io_uring m_Ring;
			std::mutex m_RingMutex;
			std::stop_source m_StopSource;
			std::thread m_DispatchThread;
			int32 m_InitializationError = 0;

		public:
			static LinuxSocketAsyncQueue& Get()
			{
				static LinuxSocketAsyncQueue queue;
				return queue;
			}

			LinuxSocketAsyncQueue()
			{
				int initResult = io_uring_queue_init(static_cast<unsigned int>(kQueueDepth), &m_Ring, 0);
				if (initResult != 0)
				{
					m_InitializationError = initResult < 0 ? -initResult : initResult;
					return;
				}

				m_DispatchThread = std::thread([this]() { Dispatch(m_StopSource.get_token()); });
			}

			~LinuxSocketAsyncQueue() noexcept
			{
				if (m_InitializationError != 0)
				{
					return;
				}

				m_StopSource.request_stop();
				QueueWakeup();
				if (m_DispatchThread.joinable())
				{
					m_DispatchThread.join();
				}
				io_uring_queue_exit(&m_Ring);
			}

			template<class TPrepare>
			bool Submit(TPrepare&& prepare, MoveOnlyFunction<void(int32)> completion)
			{
				if (m_InitializationError != 0)
				{
					errno = m_InitializationError;
					return false;
				}

				auto operation = std::make_unique<LinuxSocketOperation>(std::move(completion));

				auto lock = std::unique_lock{ m_RingMutex };
				io_uring_sqe* sqe = io_uring_get_sqe(&m_Ring);
				if (sqe == nullptr)
				{
					errno = EAGAIN;
					return false;
				}

				prepare(sqe);
				io_uring_sqe_set_data(sqe, operation.get());
				int submitResult = io_uring_submit(&m_Ring);
				if (submitResult < 0)
				{
					errno = -submitResult;
					return false;
				}

				operation.release();
				return true;
			}

		private:
			void QueueWakeup() noexcept
			{
				auto lock = std::unique_lock{ m_RingMutex };
				io_uring_sqe* sqe = io_uring_get_sqe(&m_Ring);
				if (sqe != nullptr)
				{
					io_uring_prep_nop(sqe);
					io_uring_sqe_set_data(sqe, nullptr);
					io_uring_submit(&m_Ring);
				}
			}

			void Dispatch(std::stop_token stopToken)
			{
				while (true)
				{
					io_uring_cqe* cqe = nullptr;
					int result = io_uring_wait_cqe(&m_Ring, &cqe);
					if (result == -EINTR)
					{
						continue;
					}

					if (result != 0)
					{
						if (stopToken.stop_requested())
						{
							break;
						}
						continue;
					}

					auto* operation = reinterpret_cast<LinuxSocketOperation*>(cqe->user_data);
					int32 operationResult = static_cast<int32>(cqe->res);
					io_uring_cqe_seen(&m_Ring, cqe);

					if (operation == nullptr)
					{
						if (stopToken.stop_requested())
						{
							break;
						}
						continue;
					}

					ThreadPool::QueueUserWorkItem([operation, operationResult]()
					{
						std::unique_ptr<LinuxSocketOperation> ownedOperation(operation);
						ownedOperation->Complete(operationResult);
					});
				}
			}
		};
#elif PLATFORM_OSX
		class OSXSocketOperation
		{
			int m_Socket = INVALID_SOCKET;
			int16 m_Filter = 0;
			MoveOnlyFunction<bool(int32)> m_Completion;

		public:
			OSXSocketOperation(int socket, int16 filter, MoveOnlyFunction<bool(int32)> completion)
				: m_Socket(socket)
				, m_Filter(filter)
				, m_Completion(std::move(completion))
			{
			}

			int GetSocket() const noexcept { return m_Socket; }
			int16 GetFilter() const noexcept { return m_Filter; }

			bool Complete(int32 error)
			{
				return m_Completion(error);
			}
		};

		class OSXSocketAsyncQueue
		{
			static constexpr uintptr_t kWakeIdent = 1;

			int m_Kqueue = -1;
			std::mutex m_KqueueMutex;
			std::stop_source m_StopSource;
			std::thread m_DispatchThread;
			int32 m_InitializationError = 0;

		public:
			static OSXSocketAsyncQueue& Get()
			{
				static OSXSocketAsyncQueue queue;
				return queue;
			}

			OSXSocketAsyncQueue()
			{
				m_Kqueue = kqueue();
				if (m_Kqueue < 0)
				{
					m_InitializationError = errno;
					return;
				}

				struct kevent wakeEvent;
				EV_SET(&wakeEvent, kWakeIdent, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, nullptr);
				if (kevent(m_Kqueue, &wakeEvent, 1, nullptr, 0, nullptr) != 0)
				{
					m_InitializationError = errno;
					close(m_Kqueue);
					m_Kqueue = -1;
					return;
				}

				m_DispatchThread = std::thread([this]() { Dispatch(m_StopSource.get_token()); });
			}

			~OSXSocketAsyncQueue() noexcept
			{
				if (m_InitializationError != 0)
				{
					return;
				}

				m_StopSource.request_stop();
				QueueWakeup();
				if (m_DispatchThread.joinable())
				{
					m_DispatchThread.join();
				}
				if (m_Kqueue >= 0)
				{
					close(m_Kqueue);
				}
			}

			bool Submit(int socket, int16 filter, MoveOnlyFunction<bool(int32)> completion)
			{
				if (m_InitializationError != 0)
				{
					errno = m_InitializationError;
					return false;
				}

				auto operation = std::make_unique<OSXSocketOperation>(socket, filter, std::move(completion));
				if (!Register(operation.get()))
				{
					return false;
				}

				operation.release();
				return true;
			}

		private:
			bool Register(OSXSocketOperation* operation) noexcept
			{
				struct kevent event;
				EV_SET(&event, operation->GetSocket(), operation->GetFilter(), EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, operation);

				auto lock = std::unique_lock{ m_KqueueMutex };
				return kevent(m_Kqueue, &event, 1, nullptr, 0, nullptr) == 0;
			}

			void QueueWakeup() noexcept
			{
				struct kevent event;
				EV_SET(&event, kWakeIdent, EVFILT_USER, 0, NOTE_TRIGGER, 0, nullptr);

				auto lock = std::unique_lock{ m_KqueueMutex };
				kevent(m_Kqueue, &event, 1, nullptr, 0, nullptr);
			}

			void Dispatch(std::stop_token stopToken)
			{
				while (true)
				{
					struct kevent event;
					int result = kevent(m_Kqueue, nullptr, 0, &event, 1, nullptr);
					if (result == SOCKET_ERROR)
					{
						if (errno == EINTR)
						{
							continue;
						}
						if (stopToken.stop_requested())
						{
							break;
						}
						continue;
					}

					if (event.filter == EVFILT_USER)
					{
						if (stopToken.stop_requested())
						{
							break;
						}
						continue;
					}

					auto* operation = reinterpret_cast<OSXSocketOperation*>(event.udata);
					if (operation == nullptr)
					{
						continue;
					}

					int32 error = (event.flags & EV_ERROR) != 0 ? static_cast<int32>(event.data) : 0;
					ThreadPool::QueueUserWorkItem([this, operation, error]()
					{
						if (operation->Complete(error))
						{
							if (Register(operation))
							{
								return;
							}
							operation->Complete(errno);
						}
						delete operation;
					});
				}
			}
		};

		inline int SetNonBlocking(int socket)
		{
			int flags = fcntl(socket, F_GETFL, 0);
			if (flags != SOCKET_ERROR)
			{
				fcntl(socket, F_SETFL, flags | O_NONBLOCK);
			}
			return flags;
		}

		inline void RestoreBlockingMode(int socket, int flags)
		{
			if (flags != SOCKET_ERROR)
			{
				fcntl(socket, F_SETFL, flags);
			}
		}
#endif
	}

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
			struct AcceptState
			{
				sockaddr_storage m_Address = {};
				socklen_t m_AddressLength = static_cast<socklen_t>(sizeof(m_Address));
			};

			auto tcs = TaskCompletionSource<std::shared_ptr<Socket>>::Create(cancellationToken);
			auto state = std::make_shared<AcceptState>();

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, state](io_uring_sqe* sqe)
				{
					io_uring_prep_accept(sqe, socket, reinterpret_cast<sockaddr*>(&state->m_Address), &state->m_AddressLength, 0);
				},
				[tcs, state](int32 result)
				{
					(void)state;
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					SetSocketResult(tcs, Socket::CreateFromPlatformSocket(std::make_unique<PlatformSocket>(result)));
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_READ,
				[socket = m_Socket, tcs, state, previousFlags](int32 eventError) mutable
				{
					if (eventError != 0)
					{
						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, eventError);
						return false;
					}

					auto clientSocket = accept(socket, reinterpret_cast<sockaddr*>(&state->m_Address), &state->m_AddressLength);
					if (clientSocket == INVALID_SOCKET)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							return true;
						}

						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, errno);
						return false;
					}

					RestoreBlockingMode(socket, previousFlags);
					SetSocketResult(tcs, Socket::CreateFromPlatformSocket(std::make_unique<PlatformSocket>(clientSocket)));
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

			return tcs.GetTask();
		}

		Task<> ConnectAsync(const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			struct ConnectState
			{
				sockaddr_storage m_Address = {};
				socklen_t m_AddressLength = 0;
			};

			auto tcs = TaskCompletionSource<>::Create(cancellationToken);
			auto state = std::make_shared<ConnectState>();
			state->m_AddressLength = static_cast<socklen_t>(IPEndPointToSockAddr(remoteEP, state->m_Address));

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, state](io_uring_sqe* sqe)
				{
					io_uring_prep_connect(sqe, socket, reinterpret_cast<sockaddr*>(&state->m_Address), state->m_AddressLength);
				},
				[this, tcs, state](int32 result)
				{
					(void)state;
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					m_IsConnected = true;
					SetSocketResult(tcs);
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			int result = connect(m_Socket, reinterpret_cast<sockaddr*>(&state->m_Address), state->m_AddressLength);
			if (result == 0)
			{
				RestoreBlockingMode(m_Socket, previousFlags);
				m_IsConnected = true;
				SetSocketResult(tcs);
				return tcs.GetTask();
			}

			if (errno != EINPROGRESS)
			{
				int32 error = errno;
				RestoreBlockingMode(m_Socket, previousFlags);
				SetSocketException(tcs, error);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_WRITE,
				[this, tcs, previousFlags](int32 eventError)
				{
					RestoreBlockingMode(m_Socket, previousFlags);
					if (eventError != 0)
					{
						SetSocketException(tcs, eventError);
						return false;
					}

					int socketError = 0;
					socklen_t socketErrorLength = sizeof(socketError);
					if (getsockopt(m_Socket, SOL_SOCKET, SO_ERROR, &socketError, &socketErrorLength) == SOCKET_ERROR)
					{
						SetSocketException(tcs, errno);
						return false;
					}

					if (socketError != 0)
					{
						SetSocketException(tcs, socketError);
						return false;
					}

					m_IsConnected = true;
					SetSocketResult(tcs);
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

			return tcs.GetTask();
		}

		Task<size_t> SendAsync(std::span<const uint8> buffer, std::stop_token cancellationToken)
		{
			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto sendBuffer = std::make_shared<std::vector<uint8>>(buffer.begin(), buffer.end());

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, sendBuffer](io_uring_sqe* sqe)
				{
					io_uring_prep_send(sqe, socket, sendBuffer->data(), sendBuffer->size(), 0);
				},
				[tcs, sendBuffer](int32 result)
				{
					(void)sendBuffer;
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					SetSocketResult(tcs, static_cast<size_t>(result));
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_WRITE,
				[socket = m_Socket, tcs, sendBuffer, previousFlags](int32 eventError)
				{
					if (eventError != 0)
					{
						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, eventError);
						return false;
					}

					auto result = send(socket, sendBuffer->data(), sendBuffer->size(), 0);
					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							return true;
						}

						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, errno);
						return false;
					}

					RestoreBlockingMode(socket, previousFlags);
					SetSocketResult(tcs, static_cast<size_t>(result));
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

			return tcs.GetTask();
		}

		Task<size_t> ReceiveAsync(std::span<uint8> buffer, std::stop_token cancellationToken)
		{
			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, buffer](io_uring_sqe* sqe)
				{
					io_uring_prep_recv(sqe, socket, buffer.data(), buffer.size_bytes(), 0);
				},
				[tcs](int32 result)
				{
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					SetSocketResult(tcs, static_cast<size_t>(result));
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_READ,
				[socket = m_Socket, tcs, buffer, previousFlags](int32 eventError)
				{
					if (eventError != 0)
					{
						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, eventError);
						return false;
					}

					auto result = recv(socket, buffer.data(), buffer.size_bytes(), 0);
					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							return true;
						}

						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, errno);
						return false;
					}

					RestoreBlockingMode(socket, previousFlags);
					SetSocketResult(tcs, static_cast<size_t>(result));
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

			return tcs.GetTask();
		}

		Task<size_t> SendToAsync(std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			struct SendMessageState
			{
				std::vector<uint8> m_Buffer;
				sockaddr_storage m_Address = {};
				socklen_t m_AddressLength = 0;
				iovec m_Iov = {};
				msghdr m_Message = {};

				SendMessageState(std::span<const uint8> buffer, const IPEndPoint& remoteEP)
					: m_Buffer(buffer.begin(), buffer.end())
				{
					m_AddressLength = static_cast<socklen_t>(IPEndPointToSockAddr(remoteEP, m_Address));
					m_Iov =
					{
						.iov_base = m_Buffer.data(),
						.iov_len = m_Buffer.size()
					};
					m_Message.msg_name = &m_Address;
					m_Message.msg_namelen = m_AddressLength;
					m_Message.msg_iov = &m_Iov;
					m_Message.msg_iovlen = 1;
				}
			};

			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto state = std::make_shared<SendMessageState>(buffer, remoteEP);

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, state](io_uring_sqe* sqe)
				{
					io_uring_prep_sendmsg(sqe, socket, &state->m_Message, 0);
				},
				[tcs, state](int32 result)
				{
					(void)state;
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					SetSocketResult(tcs, static_cast<size_t>(result));
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_WRITE,
				[socket = m_Socket, tcs, state, previousFlags](int32 eventError)
				{
					if (eventError != 0)
					{
						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, eventError);
						return false;
					}

					auto result = sendto(socket, state->m_Buffer.data(), state->m_Buffer.size(), 0, reinterpret_cast<sockaddr*>(&state->m_Address), state->m_AddressLength);
					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							return true;
						}

						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, errno);
						return false;
					}

					RestoreBlockingMode(socket, previousFlags);
					SetSocketResult(tcs, static_cast<size_t>(result));
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

			return tcs.GetTask();
		}

		Task<size_t> ReceiveFromAsync(std::span<uint8> buffer, IPEndPoint& remoteEP, std::stop_token cancellationToken)
		{
			struct ReceiveMessageState
			{
				sockaddr_storage m_Address = {};
				socklen_t m_AddressLength = static_cast<socklen_t>(sizeof(m_Address));
				iovec m_Iov = {};
				msghdr m_Message = {};

				explicit ReceiveMessageState(std::span<uint8> buffer)
				{
					m_Iov =
					{
						.iov_base = buffer.data(),
						.iov_len = buffer.size_bytes()
					};
					m_Message.msg_name = &m_Address;
					m_Message.msg_namelen = m_AddressLength;
					m_Message.msg_iov = &m_Iov;
					m_Message.msg_iovlen = 1;
				}
			};

			auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
			auto state = std::make_shared<ReceiveMessageState>(buffer);

#if PLATFORM_LINUX
			bool submitted = LinuxSocketAsyncQueue::Get().Submit(
				[socket = m_Socket, state](io_uring_sqe* sqe)
				{
					io_uring_prep_recvmsg(sqe, socket, &state->m_Message, 0);
				},
				[tcs, state, &remoteEP](int32 result)
				{
					if (result < 0)
					{
						SetSocketException(tcs, -result);
						return;
					}

					remoteEP = SockAddrToIPEndPoint(state->m_Address);
					SetSocketResult(tcs, static_cast<size_t>(result));
				});
#else
			int previousFlags = SetNonBlocking(m_Socket);
			if (previousFlags == SOCKET_ERROR)
			{
				SetSocketException(tcs, errno);
				return tcs.GetTask();
			}

			bool submitted = OSXSocketAsyncQueue::Get().Submit(m_Socket, EVFILT_READ,
				[socket = m_Socket, tcs, state, &remoteEP, buffer, previousFlags](int32 eventError)
				{
					if (eventError != 0)
					{
						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, eventError);
						return false;
					}

					auto result = recvfrom(socket, buffer.data(), buffer.size_bytes(), 0, reinterpret_cast<sockaddr*>(&state->m_Address), &state->m_AddressLength);
					if (result == SOCKET_ERROR)
					{
						if (errno == EAGAIN || errno == EWOULDBLOCK)
						{
							return true;
						}

						RestoreBlockingMode(socket, previousFlags);
						SetSocketException(tcs, errno);
						return false;
					}

					RestoreBlockingMode(socket, previousFlags);
					remoteEP = SockAddrToIPEndPoint(state->m_Address);
					SetSocketResult(tcs, static_cast<size_t>(result));
					return false;
				});
#endif

			if (!submitted)
			{
#if PLATFORM_OSX
				RestoreBlockingMode(m_Socket, previousFlags);
#endif
				SetSocketException(tcs, GetSubmitFailureError());
			}

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
