// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

namespace Ayla
{
	template<class T = void>
	struct PropertyGather
	{
		static_assert(std::same_as<T, void>);
	};

	template<>
	struct PropertyGather<void>
	{
	protected:
		PropertyGather()
		{
		}

	public:
		virtual ~PropertyGather() noexcept
		{
		}

		virtual void PullPPtrs(std::vector<Object*>& output) = 0;
	};
}