// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Reflection/PPtrGather.h"
#include <memory>
#include <span>

namespace Ayla
{
	struct GC;

	class PropertyCollector
	{
		friend GC;
		std::vector<std::unique_ptr<PPtrGather<void>>> m_PPtrMembers;

	public:
		PropertyCollector() noexcept = default;
		PropertyCollector(const PropertyCollector&) = delete;

		template<class T>
		inline void Add(String name, T& pptr) requires requires
		{
			std::declval<PPtrGather<T>>();
		}
		{
			m_PPtrMembers.emplace_back(std::make_unique<PPtrGather<T>>(&pptr));
		}

		PropertyCollector& operator =(const PropertyCollector&) = delete;
	};
}