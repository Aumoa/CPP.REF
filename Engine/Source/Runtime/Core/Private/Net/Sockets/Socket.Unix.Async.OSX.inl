// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IntegralTypes.h"
#include "MoveOnlyFunction.h"
#include "Threading/ThreadPool.h"
#include <sys/event.h>
#include <sys/socket.h>
#include <atomic>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <stop_token>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Ayla
{
	namespace
	{
		class OSXSocketAsyncQueue;

		class OSXSocketOperation
		{
			using cancellation_callback_t = std::stop_callback<MoveOnlyFunction<void()>>;

			uintptr_t m_Id = 0;
			OSXSocketAsyncQueue* m_Queue = nullptr;
			int m_Socket = INVALID_SOCKET;
			int16 m_Filter = 0;
			MoveOnlyFunction<bool(int32)> m_Completion;
			std::optional<cancellation_callback_t> m_Cancellation;

		public:
			OSXSocketOperation(uintptr_t id, OSXSocketAsyncQueue& queue, int socket, int16 filter, MoveOnlyFunction<bool(int32)> completion, std::stop_token cancellationToken);

			uintptr_t GetId() const noexcept { return m_Id; }
			int GetSocket() const noexcept { return m_Socket; }
			int16 GetFilter() const noexcept { return m_Filter; }

			bool Complete(int32 error)
			{
				bool retry = m_Completion(error);
				if (!retry)
				{
					m_Cancellation.reset();
				}
				return retry;
			}
		};

		class OSXSocketAsyncQueue
		{
			static constexpr uintptr_t kWakeIdent = 1;

			int m_Kqueue = -1;
			std::mutex m_KqueueMutex;
			std::mutex m_OperationsMutex;
			std::unordered_map<uintptr_t, std::unique_ptr<OSXSocketOperation>> m_Operations;
			std::atomic<uintptr_t> m_NextOperationId = 1;
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

			bool Submit(int socket, int16 filter, MoveOnlyFunction<bool(int32)> completion, std::stop_token cancellationToken)
			{
				if (m_InitializationError != 0)
				{
					errno = m_InitializationError;
					return false;
				}
				if (cancellationToken.stop_requested())
				{
					errno = ECANCELED;
					return false;
				}

				uintptr_t operationId = m_NextOperationId.fetch_add(1, std::memory_order_relaxed);
				if (operationId == 0)
				{
					operationId = m_NextOperationId.fetch_add(1, std::memory_order_relaxed);
				}

				auto operation = std::make_unique<OSXSocketOperation>(operationId, *this, socket, filter, std::move(completion), cancellationToken);
				if (!QueueOperation(operation))
				{
					return false;
				}

				if (cancellationToken.stop_requested())
				{
					Cancel(operationId);
				}

				return true;
			}

			void Cancel(uintptr_t operationId) noexcept
			{
				std::unique_ptr<OSXSocketOperation> operation = ExtractOperation(operationId);
				if (!operation)
				{
					return;
				}

				DeleteEvent(*operation);
				ThreadPool::QueueUserWorkItem([operation = std::move(operation)]() mutable
				{
					operation->Complete(ECANCELED);
				});
			}

		private:
			bool QueueOperation(std::unique_ptr<OSXSocketOperation>& operation) noexcept
			{
				OSXSocketOperation* operationPtr = operation.get();
				auto lock = std::unique_lock{ m_OperationsMutex };
				m_Operations.emplace(operationPtr->GetId(), std::move(operation));

				if (Register(*operationPtr))
				{
					return true;
				}

				auto iter = m_Operations.find(operationPtr->GetId());
				if (iter != m_Operations.end())
				{
					operation = std::move(iter->second);
					m_Operations.erase(iter);
				}
				return false;
			}

			std::unique_ptr<OSXSocketOperation> ExtractOperation(uintptr_t operationId) noexcept
			{
				auto lock = std::unique_lock{ m_OperationsMutex };
				auto iter = m_Operations.find(operationId);
				if (iter == m_Operations.end())
				{
					return nullptr;
				}

				std::unique_ptr<OSXSocketOperation> operation = std::move(iter->second);
				m_Operations.erase(iter);
				return operation;
			}

			bool Register(OSXSocketOperation& operation) noexcept
			{
				struct kevent event;
				EV_SET(&event, operation.GetSocket(), operation.GetFilter(), EV_ADD | EV_ENABLE | EV_ONESHOT, 0, 0, reinterpret_cast<void*>(operation.GetId()));

				auto lock = std::unique_lock{ m_KqueueMutex };
				return kevent(m_Kqueue, &event, 1, nullptr, 0, nullptr) == 0;
			}

			void DeleteEvent(const OSXSocketOperation& operation) noexcept
			{
				struct kevent event;
				EV_SET(&event, operation.GetSocket(), operation.GetFilter(), EV_DELETE, 0, 0, nullptr);

				auto lock = std::unique_lock{ m_KqueueMutex };
				kevent(m_Kqueue, &event, 1, nullptr, 0, nullptr);
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

					uintptr_t operationId = reinterpret_cast<uintptr_t>(event.udata);
					if (operationId == 0)
					{
						continue;
					}

					std::unique_ptr<OSXSocketOperation> operation = ExtractOperation(operationId);
					if (!operation)
					{
						continue;
					}

					int32 error = (event.flags & EV_ERROR) != 0 ? static_cast<int32>(event.data) : 0;
					ThreadPool::QueueUserWorkItem([this, operation = std::move(operation), error]() mutable
					{
						if (operation->Complete(error))
						{
							if (QueueOperation(operation))
							{
								return;
							}
							operation->Complete(errno);
						}
					});
				}
			}
		};

		OSXSocketOperation::OSXSocketOperation(uintptr_t id, OSXSocketAsyncQueue& queue, int socket, int16 filter, MoveOnlyFunction<bool(int32)> completion, std::stop_token cancellationToken)
			: m_Id(id)
			, m_Queue(&queue)
			, m_Socket(socket)
			, m_Filter(filter)
			, m_Completion(std::move(completion))
		{
			if (cancellationToken.stop_possible())
			{
				m_Cancellation.emplace(cancellationToken, [this]()
				{
					m_Queue->Cancel(m_Id);
				});
			}
		}

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
					},
					cancellationToken);

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
					},
					cancellationToken);

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
					},
					cancellationToken);

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
					},
					cancellationToken);

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
					},
					cancellationToken);

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
					},
					cancellationToken);

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
