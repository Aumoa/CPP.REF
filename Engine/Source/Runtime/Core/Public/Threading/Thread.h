// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "ISuspendToken.h"
#include "Misc/NonCopyable.h"
#include "Misc/PlatformMacros.h"
#include <string>
#include <future>
#include <functional>
#include <optional>
#include <mutex>
#include <condition_variable>

namespace libty::inline Core::inline Threading
{
	class CORE_API Thread : public NonCopyable
	{
	public:
		class CORE_API ThreadSuspendToken : public ISuspendToken
		{
			friend class Thread;
			Thread* CurrentThread;

			std::optional<std::promise<void>> SuspendPromise;

		private:
			ThreadSuspendToken(Thread* CurrentThread);

		public:
			virtual ~ThreadSuspendToken() noexcept override = default;

			virtual std::future<void> Suspend() override;
			virtual void Resume() override;

			void Join();
		};

	#if PLATFORM_WINDOWS
		void* ThreadHandle = nullptr;
	#endif

		std::thread::id ThreadId;
		std::wstring FriendlyName;
		bool bIsManaged = false;
		ThreadSuspendToken* SToken = nullptr;

		std::promise<void> JoinPromise;
		std::future<void> JoinFuture;
		std::mutex SuspendMtx;
		std::condition_variable SuspendCv;

	private:
		Thread();

	public:
		~Thread();

		void SetFriendlyName(std::wstring_view InFriendlyName);
		void SuspendThread();
		void ResumeThread();
		void Join();

		std::wstring GetFriendlyName() const;
		std::thread::id GetThreadId() const;
		bool IsManaged() const;
		ThreadSuspendToken* GetSuspendToken() const;

		static Thread* CreateThread(std::wstring_view FriendlyName, std::function<void()> ThreadEntry);

	public:
		static Thread* GetCurrentThread();
	};
}