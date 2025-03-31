// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"

namespace Ayla
{
	class LogCategory
	{
	private:
		String m_Name;

	public:
		constexpr LogCategory(String name) noexcept
			: m_Name(std::move(name))
		{
		}

		constexpr String GetName() const noexcept
		{
			return m_Name;
		}
	};
}