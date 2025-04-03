// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "PropertyCollector.h"
#include <typeinfo>

namespace Ayla
{
	struct CORE_API TypeRegister
	{
		String Namespace;
		String Name;
		String FullName;
		const std::type_info* TypeInfo;
		PropertyCollector PCollector;

		void Register() const;

		template<class T>
		static void GetPropertyCollector(PropertyCollector& collector)
		{
			T::GatherProperties(collector);
		}
	};
}