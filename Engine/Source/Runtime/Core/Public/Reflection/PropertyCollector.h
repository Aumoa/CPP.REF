// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/reflexpr.h"
#include "SharedPtr.h"
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
		std::experimental::reflect::access_type m_DefaultConstructorAccess = std::experimental::reflect::access_type::public_;
		SharedPtr<Object>(*m_DefaultConstructor)();

	public:
		PropertyCollector() noexcept = default;
		PropertyCollector(const PropertyCollector&) = delete;

		template<std::experimental::reflect::is_reflexpr_field T>
		inline void Transfer()
		{
		}

		template<std::experimental::reflect::is_reflexpr_method T>
		inline void Transfer()
		{
		}

		template<std::experimental::reflect::is_reflexpr_constructor T>
		inline void Transfer()
		{
			if constexpr (std::invocable<typename T::function_type>)
			{
				m_DefaultConstructorAccess = std::experimental::reflect::get_access_v<T>;
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