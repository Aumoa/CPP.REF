// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/reflexpr.h"
#include <memory>
#include <span>

namespace Ayla
{
	class PropertyCollector
	{
	public:
		PropertyCollector() noexcept = default;
		PropertyCollector(const PropertyCollector&) = delete;

		template<std::reflect::is_reflexpr_field T>
		inline void Transfer()
		{
		}

		template<std::reflect::is_reflexpr_method T>
		inline void Transfer()
		{
		}

		template<std::reflect::is_reflexpr_constructor T>
		inline void Transfer()
		{
		}

		PropertyCollector& operator =(const PropertyCollector&) = delete;
	};
}