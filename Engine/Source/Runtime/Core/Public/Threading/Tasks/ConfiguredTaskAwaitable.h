// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Tasks/ConfiguredTaskAwaiter.h"

namespace Ayla
{
	template<class T>
	class ConfiguredTaskAwaitable
	{
		const std::shared_ptr<SharedTask<T>> m_Task;
		const bool m_ContinueOnCapturedContext;

	public:
		ConfiguredTaskAwaitable(std::shared_ptr<SharedTask<T>> task, bool continueOnCapturedContext) noexcept
			: m_Task(std::move(task))
			, m_ContinueOnCapturedContext(continueOnCapturedContext)
		{
		}

		ConfiguredTaskAwaiter<T> GetAwaiter() const noexcept
		{
			return ConfiguredTaskAwaiter<T>(m_Task, m_ContinueOnCapturedContext);
		}
	};
}