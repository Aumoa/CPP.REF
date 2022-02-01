// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <optional>

namespace Threading::Tasks
{
	template<class U>
	class VoidableOptional : public std::optional<U>
	{
	};

	template<>
	class VoidableOptional<void>
	{
		bool bHasValue = false;

	public:
		bool has_value() const
		{
			return bHasValue;
		}

		void emplace()
		{
			bHasValue = true;
		}

		void value()
		{
		}

		void reset()
		{
			bHasValue = false;
		}
	};
}