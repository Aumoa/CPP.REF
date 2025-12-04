// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"

namespace Ayla
{
	class CORE_API AggregateException : public Exception
	{
		const std::vector<std::exception_ptr> m_InnerExceptions;

	public:
		template<std::ranges::input_range IR>
		AggregateException(IR&& exceptions) requires std::convertible_to<std::ranges::range_value_t<IR>, std::exception_ptr>
			: m_InnerExceptions(std::ranges::to<std::vector>(std::forward<IR>(exceptions)))
		{
		}

		virtual ~AggregateException() noexcept override;

		std::span<const std::exception_ptr> GetInnerExceptions() const noexcept
		{
			return m_InnerExceptions;
		}

		std::exception_ptr GetInnerException() const noexcept override
		{
			if (m_InnerExceptions.empty())
			{
				return {};
			}
			return m_InnerExceptions.front();
		}
	};
}