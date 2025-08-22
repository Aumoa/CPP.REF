// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <type_traits>
#include "String_.h"
#include "AssertionMacros.h"

namespace Ayla
{
	template<std::invocable<> TTry>
	class [[nodiscard]] try_
	{
	private:
		TTry m_Try;
		bool m_Used;

	public:
		template<std::invocable<> U>
		try_(U&& try_)
			: m_Try{ std::forward<U>(try_) }
			, m_Used{ false }
		{
		}

		~try_() noexcept
		{
			checkf(m_Used, TEXT("The 'finally_' function must be contiguous."));
		}

		template<std::invocable<> U>
		auto finally_(U&& finally_)
		{
			struct finally_caller
			{
				U m_Finally;

				finally_caller(U&& finally_) : m_Finally{ std::forward<U>(finally_) }
				{
				}

				~finally_caller() noexcept
				{
					m_Finally();
				}
			} finally_invoke(std::forward<U>(finally_));

			m_Used = true;
			return m_Try();
		}
	};

	template<std::invocable<> T>
	try_(T) -> try_<T>;
}

#define try__ try_([&]() { try
#define finally__ catch (...) { throw; } }).finally_([&]()
#define end_try__ )