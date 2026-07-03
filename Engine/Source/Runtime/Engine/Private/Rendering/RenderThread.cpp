// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Rendering/RenderThread.h"
#include "Threading/Thread.h"
#include "Graphics.h"

namespace Ayla
{
	RenderThread* RenderThread::m_Current;

	RenderThread::RenderThread(SharedPtr<Graphics> graphics)
		: m_Thread(std::bind(&RenderThread::ThreadProc, this, std::move(graphics)))
	{
		m_Current = this;
	}

	RenderThread::~RenderThread() noexcept
	{
	}

	void RenderThread::Add(function_t<void()> job)
	{
		auto lock = std::unique_lock{ m_Mtx };
		if (m_Exception)
		{
			std::rethrow_exception(m_Exception);
		}

		m_Jobs.emplace(std::move(job));
	}

	void RenderThread::Dispatch(function_t<void()> completionAction)
	{
		auto lock = std::unique_lock{ m_Mtx };
		if (m_Exception)
		{
			std::rethrow_exception(m_Exception);
		}

		m_Notify.wait(lock, [&]() { return m_CompletionActions.size() < 2 || m_Exception || m_StopRequested; });
		if (m_Exception)
		{
			std::rethrow_exception(m_Exception);
		}

		m_CompletionActions.emplace(std::move(completionAction));
		m_Request.notify_one();
	}

	void RenderThread::AddAfterCompleted(function_t<void()> completionAction)
	{
		auto lock = std::unique_lock{ m_Mtx };
		if (m_Exception)
		{
			std::rethrow_exception(m_Exception);
		}

		m_AfterCompletedActions.emplace(std::move(completionAction));
	}

	void RenderThread::RequestStop()
	{
		auto lock = std::unique_lock{ m_Mtx };
		m_StopRequested = true;
		m_Request.notify_one();
		lock.unlock();
		m_Thread.join();

		{
			std::queue<function_t<void()>> empty;
			std::swap(m_CompletionActions, empty);
		}
		{
			std::queue<function_t<void()>> empty;
			std::swap(m_Jobs, empty);
		}
	}

	void RenderThread::ExecuteJobs()
	{
		auto lock = std::unique_lock{ m_Mtx };
		static std::queue<function_t<void()>> executionJobs;
		while (m_Jobs.empty() == false)
		{
			executionJobs.emplace(std::move(m_Jobs.front()));
			m_Jobs.pop();
		}

		lock.unlock();

		while (!executionJobs.empty())
		{
			executionJobs.front()();
			executionJobs.pop();
		}
	}

	void RenderThread::ThreadProc(SharedPtr<Graphics> graphics)
	{
		Thread::GetCurrentThread().SetDescription(TEXT("Render Thread #0"));

		try
		{
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
				static std::queue<function_t<void()>> afterCompletedActions;
				while (m_AfterCompletedActions.empty() == false)
				{
					afterCompletedActions.emplace(std::move(m_AfterCompletedActions.front()));
					m_AfterCompletedActions.pop();
				}

				lock.unlock();

				for (size_t i = 0; i < afterCompletedActions.size(); i++)
				{
					afterCompletedActions.front()();
					afterCompletedActions.pop();
				}

				lock.lock();
				m_Notify.notify_one();
			}
		}
		catch (...)
		{
			auto lock = std::unique_lock{ m_Mtx };
			m_Exception = std::current_exception();
			m_StopRequested = true;
			lock.unlock();
			m_Notify.notify_all();
			m_Request.notify_all();
		}
	}
}
