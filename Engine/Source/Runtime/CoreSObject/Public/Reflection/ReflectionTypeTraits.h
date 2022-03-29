// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"

class SObject;

namespace libty::Core::Reflection
{
	namespace Details
	{
		template<class T>
		struct RemoveObjectPointer
		{
			using Type = T;
		};

		template<std::derived_from<SObject> T>
		struct RemoveObjectPointer<T*>
		{
			using Type = T;
		};
	}

	template<class T>
	using RemoveObjectPointer_t = typename Details::RemoveObjectPointer<T>::Type;

	template<class T>
	concept IEnum = std::is_enum_v<typename T::__Tag__>;
}