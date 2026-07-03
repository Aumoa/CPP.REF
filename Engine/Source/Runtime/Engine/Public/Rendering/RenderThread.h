// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoveOnlyFunction.h"
#include "RenderThread.gen.h"
#include <exception>

namespace Ayla
{
	class Graphics;

	ACLASS()
	class ENGINE_API RenderThread : public Object
	{
		GENERATED_BODY()

	public:
		template<class TBody>
		using function_t = MoveOnlyFunction<TBody>;

	private:
		static RenderThread* m_Current;

		std::thread m_Thread;
		std::mutex m_Mtx;
		std::condition_variable m_Request;
		std::condition_variable m_Notify;
		std::queue<function_t<void()>> m_Jobs;
		std::queue<function_t<void()>> m_CompletionActions;
		std::queue<function_t<void()>> m_AfterCompletedActions;
		std::exception_ptr m_Exception;
		std::atomic<bool> m_StopRequested = false;

	public:
		RenderThread(SharedPtr<Graphics> graphics);
		~RenderThread() noexcept;

		void Add(function_t<void()> job);
		void Dispatch(function_t<void()> completionAction);
		void AddAfterCompleted(function_t<void()> completionAction);

		void RequestStop();
		void ExecuteJobs();

	public:
		static inline RenderThread* GetCurrent() { return m_Current; }

	private:
		void ThreadProc(SharedPtr<Graphics> graphics);
	};
}

#define ENQUEUE_RENDER_THREAD_JOB(job) ::Ayla::RenderThread::GetCurrent()->Add((job));
#define ENQUEUE_RENDER_THREAD_FINISHED_JOB(action) ::Ayla::RenderThread::GetCurrent()->AddAfterCompleted((action));
