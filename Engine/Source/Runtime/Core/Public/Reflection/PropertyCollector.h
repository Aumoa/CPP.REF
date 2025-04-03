// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/PPtrGather.h"
#include <memory>
#include <span>

namespace Ayla
{
	struct GC;
	class BasePtr;

	class PropertyCollector
	{
		friend GC;
		std::vector<std::pair<PPtrGather<>*, size_t>> m_PPtrMembers;

	public:
		PropertyCollector() noexcept = default;
		PropertyCollector(const PropertyCollector&) = delete;

		template<std::derived_from<BasePtr> T>
		inline void Add(String name, T* offset)
		{
			m_PPtrMembers.emplace_back(PPtrGather<>::template Get<T>(), (size_t)offset);
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