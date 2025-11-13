// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderThread.gen.h"

namespace Ayla
{
	class Graphics;

	ACLASS()
	class ENGINE_API RenderThread : public Object
	{
		GENERATED_BODY()

	private:
		std::thread m_Thread;
		std::mutex m_Mtx;
		std::condition_variable m_Request;
		std::condition_variable m_Notify;
		std::queue<std::move_only_function<void()>> m_CompletionActions;
		std::atomic<bool> m_StopRequested = false;

	public:
		RenderThread(SharedPtr<Graphics> graphics);
		~RenderThread() noexcept;

		void Dispatch(std::move_only_function<void()> completionAction);
		void RequestStop();

	private:
		void ThreadProc(SharedPtr<Graphics> graphics);
	};
}