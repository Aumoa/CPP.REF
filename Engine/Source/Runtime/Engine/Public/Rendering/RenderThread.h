// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class Graphics;

	class ENGINE_API RenderThread
	{
	private:
		std::thread m_Thread;
		std::mutex m_Mtx;
		std::condition_variable m_Cv;
		std::queue<std::move_only_function<void()>> m_CompletionActions;
		std::atomic<bool> m_StopRequested = false;

	public:
		RenderThread(std::shared_ptr<Graphics> graphics);
		~RenderThread() noexcept;

		void Dispatch(std::move_only_function<void()> completionAction);
		void Join();

	private:
		void ThreadProc(std::shared_ptr<Graphics> graphics);
	};
}