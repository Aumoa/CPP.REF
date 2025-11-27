// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "TimerManager.h"

namespace Ayla
{
	TimerManager::Interval::Interval(function_t<void()> func, const TimeSpan& interval, IntervalMode mode)
		: m_Func(std::move(func))
		, m_Interval(interval)
		, m_Mode(mode)
	{
	}

	void TimerManager::Interval::Update(TimerManager* tm)
	{
		if (m_ExceptionPtr)
		{
			return;
		}

		auto timeSinceStartup = tm->m_TimeSinceStartup;
		auto distance = timeSinceStartup - m_LastTime;
		if (distance >= m_Interval)
		{
			switch (m_Mode)
			{
				case IntervalMode::Absolute:
					m_LastTime += m_Interval;
					break;
				case IntervalMode::Relative:
					m_LastTime = timeSinceStartup;
					break;
			}

			try
			{
				m_Func();
			}
			catch (...)
			{
				m_ExceptionPtr = std::current_exception();
			}
		}
	}

	void TimerManager::Interval::Throw()
	{
		if (m_ExceptionPtr)
		{
			std::rethrow_exception(m_ExceptionPtr);
		}
	}

	void TimerManager::Start()
	{
		m_Timer.Start();
		m_TimeSinceStartup = {};
		m_DeltaTime = {};
	}

	void TimerManager::StartFrame()
	{
		auto elapsed = m_Timer.GetElapsed();
		m_DeltaTime = elapsed - m_TimeSinceStartup;
		m_TimeSinceStartup = elapsed;
	}

	void TimerManager::UpdateTasks()
	{
		std::unique_lock lock(m_Mutex);
		for (auto& [_, task] : m_Tasks)
		{
			m_Buffer.emplace_back(task);
		}

		lock.unlock();

		for (auto& task : m_Buffer)
		{
			task->Update(this);
		}

		m_Buffer.clear();
	}

	int64 TimerManager::AddInterval(function_t<void()> func, const TimeSpan& interval, IntervalMode mode)
	{
		std::unique_lock lock(m_Mutex);
		int64 taskId = ++m_TaskId;
		m_Tasks.emplace(taskId, std::make_shared<Interval>(std::move(func), interval, mode));
		return taskId;
	}

	void TimerManager::RemoveTask(int64 taskId)
	{
		std::unique_lock lock(m_Mutex);
		auto it = m_Tasks.find(taskId);
		if (it != m_Tasks.end())
		{
			m_Tasks.erase(it);
			it->second->Throw();
		}
	}
}