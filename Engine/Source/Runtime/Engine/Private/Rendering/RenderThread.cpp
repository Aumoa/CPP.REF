// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/RenderThread.h"
#include "Threading/Thread.h"
#include "Graphics.h"

namespace Ayla
{
	RenderThread::RenderThread(std::shared_ptr<Graphics> graphics)
		: m_Thread(std::bind(&RenderThread::ThreadProc, this, std::move(graphics)))
	{
	}

	RenderThread::~RenderThread()
	{
	}

	void RenderThread::Dispatch(std::move_only_function<void()> completionAction)
	{
		auto lock = std::unique_lock{ m_Mtx };
		m_CompletionActions.emplace(std::move(completionAction));
		m_Cv.notify_one();
	}

	void RenderThread::Join()
	{
		auto lock = std::unique_lock{ m_Mtx };
		m_StopRequested = true;
		m_Cv.notify_one();
		lock.unlock();
		m_Thread.join();
	}

	void RenderThread::ThreadProc(std::shared_ptr<Graphics> graphics)
	{
		Thread::GetCurrentThread().SetDescription(TEXT("Render Thread #0"));

		while (m_StopRequested == false)
		{
			auto lock = std::unique_lock{ m_Mtx };
			while (m_CompletionActions.empty())
			{
				m_Cv.wait(lock);
				if (m_StopRequested)
				{
					break;
				}
			}

			auto completionAction = std::move(m_CompletionActions.front());
			m_CompletionActions.pop();

			lock.unlock();

			completionAction();
		}
	}
}