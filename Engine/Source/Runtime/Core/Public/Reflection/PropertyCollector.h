// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/PPtrGather.h"
#include "Reflection/reflexpr.h"
#include <memory>
#include <span>

namespace Ayla
{
	class GC;
	class BasePtr;

	class PropertyCollector
	{
		friend GC;
		std::vector<std::pair<PPtrGather<>*, size_t>> m_PPtrMembers;

	public:
		PropertyCollector() noexcept = default;
		PropertyCollector(const PropertyCollector&) = delete;

		template<std::reflect::is_reflexpr_field T>
		inline void Transfer()
		{
			using element_t = std::reflect::get_field_type_t<T>;
			if constexpr (std::derived_from<element_t, Object>)
			{
				using ptr_t = PPtr<element_t>;
				m_PPtrMembers.emplace_back(PPtrGather<>::template Get<ptr_t>(), std::reflect::get_field_offset_v<T>);
			}
		}

		template<std::reflect::is_reflexpr_method T>
		inline void Transfer()
		{
		}

		PropertyCollector& operator =(const PropertyCollector&) = delete;

		template<class U, class T>
		static T* Advance(T* pointer)
		{
			size_t advance = reinterpret_cast<size_t>(static_cast<Object*>(reinterpret_cast<U*>(0)));
			return reinterpret_cast<T*>(reinterpret_cast<byte*>(pointer) + advance);
		}
	};
}