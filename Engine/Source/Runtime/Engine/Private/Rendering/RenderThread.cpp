// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/RenderThread.h"
#include "Threading/Thread.h"
#include "Graphics.h"

namespace Ayla
{
	RenderThread::RenderThread(SharedPtr<Graphics> graphics)
		: m_Thread(std::bind(&RenderThread::ThreadProc, this, std::move(graphics)))
	{
	}

	RenderThread::~RenderThread()
	{
	}

	void RenderThread::Dispatch(std::move_only_function<void()> completionAction)
	{
		auto lock = std::unique_lock{ m_Mtx };
		m_Notify.wait(lock, [&]() { return m_CompletionActions.size() < 2; });
		m_CompletionActions.emplace(std::move(completionAction));
		m_Request.notify_one();
	}

	void RenderThread::RequestStop()
	{
		auto lock = std::unique_lock{ m_Mtx };
		m_StopRequested = true;
		m_Request.notify_one();
		lock.unlock();
		m_Thread.join();
	}

	void RenderThread::ThreadProc(SharedPtr<Graphics> graphics)
	{
		Thread::GetCurrentThread().SetDescription(TEXT("Render Thread #0"));

		while (m_StopRequested == false)
		{
			auto lock = std::unique_lock{ m_Mtx };
			m_Request.wait(lock, [&]() { return !m_CompletionActions.empty() || m_StopRequested; });
			if (m_StopRequested)
			{
				break;
			}

			auto completionAction = std::move(m_CompletionActions.front());
			m_CompletionActions.pop();

			lock.unlock();

			completionAction();

			lock.lock();
			m_Notify.notify_one();
		}
	}
}