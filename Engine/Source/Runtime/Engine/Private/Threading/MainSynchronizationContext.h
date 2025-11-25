// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

namespace Ayla
{
	class MainSynchronizationContext : public SynchronizationContext
	{
	private:
		std::mutex m_Mutex;
		std::queue<SharedTask<>::function_t<void()>> m_Continuations;
		std::queue<SharedTask<>::function_t<void()>> m_ExecutionBuffer;

	public:
		virtual void Post(SharedTask<>::function_t<void()> continuation) override
		{
			std::unique_lock lock(m_Mutex);
			m_Continuations.emplace(std::move(continuation));
		}

		void Tick()
		{
			std::unique_lock lock(m_Mutex);
			std::swap(m_ExecutionBuffer, m_Continuations);
			lock.unlock();
			
			while (m_ExecutionBuffer.empty() == false)
			{
				m_ExecutionBuffer.front()();
				m_ExecutionBuffer.pop();
			}
		}

		inline static MainSynchronizationContext* GetCurrent()
		{
			auto* context = SynchronizationContext::GetCurrent();
			check(dynamic_cast<MainSynchronizationContext*>(context));
			return (MainSynchronizationContext*)context;
		}
	};
}