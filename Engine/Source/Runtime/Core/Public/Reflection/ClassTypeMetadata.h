// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ReflectionTraits.h"

namespace libty::reflect
{
	struct ClassTypeMetadata
	{
		String FriendlyName;
		std::vector<constructor_t> Constructors;

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