// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <vector>

namespace Ayla
{
	class Object;

	template<class T = void>
	struct PPtrGather
	{
		static_assert(std::same_as<T, void>);
	};

	template<>
	struct PPtrGather<void>
	{
	protected:
		PPtrGather()
		{
		}

	public:
		virtual ~PPtrGather() noexcept
		{
		}

		virtual void PullPPtrs(std::vector<Object*>& output) = 0;
	};
}