// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "reflexpr.h"
#include "Misc/String.h"
#include "Reflection/ReflectionTraits.h"
#include <memory>

namespace libty::reflect
{
	struct ClassTypeMetadata
	{
		String FriendlyName;
		std::vector<constructor_t> Constructors;
		std::vector<property_info_t> Properties;
		std::unique_ptr<ClassTypeMetadata> Super;

		template<class T>
		static ClassTypeMetadata Generate()
		{
			ClassTypeMetadata M;
			M.FriendlyName = get_friendly_name_v<T>;
			M.Constructors = T::constructors;
			M.Properties = T::properties;
			if constexpr (libty::reflect::is_class<typename T::super_t>)
			{
				M.Super = std::make_unique<ClassTypeMetadata>(Generate<typename T::super_t>());
			}
			return M;
		}
	};
}