// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "IntegralTypes.h"
#include "MoveOnlyFunction.h"
#include "Threading/ThreadPool.h"
#include <liburing.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <atomic>
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
		class LinuxSocketAsyncQueue;

		class LinuxSocketOperation
		{
			using cancellation_callback_t = std::stop_callback<MoveOnlyFunction<void()>>;

			uint64 m_Id = 0;
			LinuxSocketAsyncQueue* m_Queue = nullptr;
			MoveOnlyFunction<void(int32)> m_Completion;
			std::optional<cancellation_callback_t> m_Cancellation;

		public:
			LinuxSocketOperation(uint64 id, LinuxSocketAsyncQueue& queue, MoveOnlyFunction<void(int32)> completion, std::stop_token cancellationToken);

			uint64 GetId() const noexcept { return m_Id; }

			void Complete(int32 result)
			{
				m_Cancellation.reset();
				m_Completion(result);
			}
		};

		class LinuxSocketAsyncQueue
		{
			static constexpr uint32 kQueueDepth = 256;

			io_uring m_Ring;
			std::mutex m_RingMutex;
			std::mutex m_OperationsMutex;
			std::unordered_map<uint64, std::shared_ptr<LinuxSocketOperation>> m_Operations;
			std::atomic<uint64> m_NextOperationId = 1;
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
			bool Submit(TPrepare&& prepare, MoveOnlyFunction<void(int32)> completion, std::stop_token cancellationToken)
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

				uint64 operationId = m_NextOperationId.fetch_add(1, std::memory_order_relaxed);
				if (operationId == 0)
				{
					operationId = m_NextOperationId.fetch_add(1, std::memory_order_relaxed);
				}

				auto operation = std::make_shared<LinuxSocketOperation>(operationId, *this, std::move(completion), cancellationToken);

				auto lock = std::unique_lock{ m_RingMutex };
				io_uring_sqe* sqe = io_uring_get_sqe(&m_Ring);
				if (sqe == nullptr)
				{
					return false;
				}

				prepare(sqe);
				io_uring_sqe_set_data(sqe, reinterpret_cast<void*>(operationId));

				{
					auto operationsLock = std::unique_lock{ m_OperationsMutex };
					m_Operations.emplace(operationId, operation);
				}

				int submitResult = io_uring_submit(&m_Ring);
				if (submitResult < 0)
				{
					int submitError = -submitResult;
					lock.unlock();
					errno = submitError;
					auto operationsLock = std::unique_lock{ m_OperationsMutex };
					m_Operations.erase(operationId);
					return false;
				}

				lock.unlock();
				if (cancellationToken.stop_requested())
				{
					Cancel(operationId);
				}

				return true;
			}

			void Cancel(uint64 operationId) noexcept
			{
				if (m_InitializationError != 0 || operationId == 0)
				{
					return;
				}

				auto lock = std::unique_lock{ m_RingMutex };
				io_uring_sqe* sqe = io_uring_get_sqe(&m_Ring);
				if (sqe != nullptr)
				{
					io_uring_prep_cancel(sqe, reinterpret_cast<void*>(operationId), 0);
					io_uring_sqe_set_data(sqe, nullptr);
					io_uring_submit(&m_Ring);
				}
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

					uint64 operationId = static_cast<uint64>(cqe->user_data);
					int32 operationResult = static_cast<int32>(cqe->res);
					io_uring_cqe_seen(&m_Ring, cqe);

					if (operationId == 0)
					{
						if (stopToken.stop_requested())
						{
							break;
						}
						continue;
					}

					std::shared_ptr<LinuxSocketOperation> operation;
					{
						auto operationsLock = std::unique_lock{ m_OperationsMutex };
						auto iter = m_Operations.find(operationId);
						if (iter != m_Operations.end())
						{
							operation = iter->second;
							m_Operations.erase(iter);
						}
					}

					if (!operation)
					{
						continue;
					}

					ThreadPool::QueueUserWorkItem([operation, operationResult]()
					{
						operation->Complete(operationResult);
					});
				}
			}
		};

		LinuxSocketOperation::LinuxSocketOperation(uint64 id, LinuxSocketAsyncQueue& queue, MoveOnlyFunction<void(int32)> completion, std::stop_token cancellationToken)
			: m_Id(id)
			, m_Queue(&queue)
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

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, state](io_uring_sqe* sqe)
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

						SetSocketResult(tcs, result);
					},
					cancellationToken);

				if (!submitted)
				{
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

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, state](io_uring_sqe* sqe)
					{
						io_uring_prep_connect(sqe, socket, reinterpret_cast<sockaddr*>(&state->m_Address), state->m_AddressLength);
					},
					[tcs, state](int32 result)
					{
						(void)state;
						if (result < 0)
						{
							SetSocketException(tcs, -result);
							return;
						}

						SetSocketResult(tcs);
					},
					cancellationToken);

				if (!submitted)
				{
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<size_t> SendAsync(int socket, std::span<const uint8> buffer, std::stop_token cancellationToken)
			{
				auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);
				auto sendBuffer = std::make_shared<std::vector<uint8>>(buffer.begin(), buffer.end());

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, sendBuffer](io_uring_sqe* sqe)
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
					},
					cancellationToken);

				if (!submitted)
				{
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}

			static Task<size_t> ReceiveAsync(int socket, std::span<uint8> buffer, std::stop_token cancellationToken)
			{
				auto tcs = TaskCompletionSource<size_t>::Create(cancellationToken);

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, buffer](io_uring_sqe* sqe)
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
					},
					cancellationToken);

				if (!submitted)
				{
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

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, state](io_uring_sqe* sqe)
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
					},
					cancellationToken);

				if (!submitted)
				{
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

				auto tcs = TaskCompletionSource<UnixSocketReceiveFromResult>::Create(cancellationToken);
				auto state = std::make_shared<ReceiveMessageState>(buffer);

				bool submitted = LinuxSocketAsyncQueue::Get().Submit(
					[socket, state](io_uring_sqe* sqe)
					{
						io_uring_prep_recvmsg(sqe, socket, &state->m_Message, 0);
					},
					[tcs, state](int32 result)
					{
						if (result < 0)
						{
							SetSocketException(tcs, -result);
							return;
						}

						SetSocketResult(tcs, UnixSocketReceiveFromResult
						{
							static_cast<size_t>(result),
							SockAddrToIPEndPoint(state->m_Address)
						});
					},
					cancellationToken);

				if (!submitted)
				{
					SetSocketException(tcs, GetSubmitFailureError());
				}

				return tcs.GetTask();
			}
		};
	}
}
