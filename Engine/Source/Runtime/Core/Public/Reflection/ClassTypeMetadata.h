// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"

namespace libty::reflect
{
	struct ClassTypeMetadata
	{
		using Constructor_t = Object*(*)(std::vector<Object*>);

		String FriendlyName;
		std::vector<Constructor_t> Constructors;

		template<class T>
		static ClassTypeMetadata Generate()
		{
			ClassTypeMetadata M;
			M.FriendlyName = get_friendly_name_v<T>;
			M.Constructors = std::vector(T::constructors.begin(), T::constructors.end());
			return M;
		}
	};
}