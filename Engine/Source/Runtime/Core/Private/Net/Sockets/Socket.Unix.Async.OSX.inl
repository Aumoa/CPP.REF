// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "MoveOnlyFunction.h"
#include "Threading/ThreadPool.h"
#include <sys/event.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <memory>
#include <mutex>
#include <span>
#include <stop_token>
#include <thread>
#include <vector>

namespace Ayla
{
	namespace
	{
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

		class UnixSocketAsyncBackend
		{
		public:
			static Task<int> AcceptAsync(int socket, std::stop_token cancellationToken)
			{
				struct AcceptState
				{
					sockaddr_storage m_Address = {};
					socklen_t m_AddressLength = static_cast<socklen_t>(sizeof(m_Address));
				};

				auto tcs = TaskCompletionSource<int>::Create(cancellationToken);
				auto state = std::make_shared<AcceptState>();
				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_READ,
					[socket, tcs, state, previousFlags](int32 eventError) mutable
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
						SetSocketResult(tcs, clientSocket);
						return false;
					});

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<> ConnectAsync(int socket, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
			{
				struct ConnectState
				{
					sockaddr_storage m_Address = {};
					socklen_t m_AddressLength = 0;
				};

				auto tcs = TaskCompletionSource<>::Create(cancellationToken);
				auto state = std::make_shared<ConnectState>();
				state->m_AddressLength = static_cast<socklen_t>(IPEndPointToSockAddr(remoteEP, state->m_Address));

				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				int result = connect(socket, reinterpret_cast<sockaddr*>(&state->m_Address), state->m_AddressLength);
				if (result == 0)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketResult(tcs);
					return tcs.GetTask();
				}

				if (errno != EINPROGRESS)
				{
					int32 error = errno;
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, error);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_WRITE,
					[socket, tcs, previousFlags](int32 eventError)
					{
						RestoreBlockingMode(socket, previousFlags);
						if (eventError != 0)
						{
							SetSocketException(tcs, eventError);
							return false;
						}

						int socketError = 0;
						socklen_t socketErrorLength = sizeof(socketError);
						if (getsockopt(socket, SOL_SOCKET, SO_ERROR, &socketError, &socketErrorLength) == SOCKET_ERROR)
						{
							SetSocketException(tcs, errno);
							return false;
						}

						if (socketError != 0)
						{
							SetSocketException(tcs, socketError);
							return false;
						}

						SetSocketResult(tcs);
						return false;
					});

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<size_t> SendAsync(int socket, std::span<const uint8> buffer, std::stop_token cancellationToken)
			{
				auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
				auto sendBuffer = std::make_shared<std::vector<uint8>>(buffer.begin(), buffer.end());
				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_WRITE,
					[socket, tcs, sendBuffer, previousFlags](int32 eventError)
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

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<size_t> ReceiveAsync(int socket, std::span<uint8> buffer, std::stop_token cancellationToken)
			{
				auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_READ,
					[socket, tcs, buffer, previousFlags](int32 eventError)
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

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<size_t> SendToAsync(int socket, std::span<const uint8> buffer, const IPEndPoint& remoteEP, std::stop_token cancellationToken)
			{
				struct SendMessageState
				{
					std::vector<uint8> m_Buffer;
					sockaddr_storage m_Address = {};
					socklen_t m_AddressLength = 0;

					SendMessageState(std::span<const uint8> buffer, const IPEndPoint& remoteEP)
						: m_Buffer(buffer.begin(), buffer.end())
					{
						m_AddressLength = static_cast<socklen_t>(IPEndPointToSockAddr(remoteEP, m_Address));
					}
				};

				auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
				auto state = std::make_shared<SendMessageState>(buffer, remoteEP);
				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_WRITE,
					[socket, tcs, state, previousFlags](int32 eventError)
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

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<UnixSocketReceiveFromResult> ReceiveFromAsync(int socket, std::span<uint8> buffer, std::stop_token cancellationToken)
			{
				struct ReceiveMessageState
				{
					sockaddr_storage m_Address = {};
					socklen_t m_AddressLength = static_cast<socklen_t>(sizeof(m_Address));
				};

				auto tcs = TaskCompletionSource<UnixSocketReceiveFromResult>::Create(cancellationToken);
				auto state = std::make_shared<ReceiveMessageState>();
				int previousFlags = SetNonBlocking(socket);
				if (previousFlags == SOCKET_ERROR)
				{
					SetSocketException(tcs, errno);
					return tcs.GetTask();
				}

				bool submitted = OSXSocketAsyncQueue::Get().Submit(socket, EVFILT_READ,
					[socket, tcs, state, buffer, previousFlags](int32 eventError)
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
						SetSocketResult(tcs, UnixSocketReceiveFromResult
						{
							static_cast<size_t>(result),
							SockAddrToIPEndPoint(state->m_Address)
						});
						return false;
					});

				if (!submitted)
				{
					RestoreBlockingMode(socket, previousFlags);
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}
		};
	}
}
