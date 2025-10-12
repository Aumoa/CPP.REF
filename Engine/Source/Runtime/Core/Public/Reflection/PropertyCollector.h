// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/reflexpr.h"
#include <memory>
#include <span>
#include <concepts>

namespace Ayla
{
	class Object;

	class PropertyCollector
	{
	private:
		// TODO:
		std::reflect::access_type m_DefaultConstructorAccess = std::reflect::access_type::public_;
		std::shared_ptr<Object>(*m_DefaultConstructor)();

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
			if constexpr (std::invocable<typename T::function_type>)
			{
				m_DefaultConstructorAccess = std::reflect::get_access_v<T>;
				m_DefaultConstructor = T::pointer;
			}
		}

		auto GetDefaultConstructor() const
		{
			return std::make_pair(m_DefaultConstructorAccess, m_DefaultConstructor);
		}

		PropertyCollector& operator =(const PropertyCollector&) = delete;
	};
}