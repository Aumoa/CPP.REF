// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

namespace Ayla
{
	template<class T>
	struct VoidableRef
	{
		using Type = T&;
	};

	template<>
	struct VoidableRef<void>
	{
		using Type = void;
	};

	template<class T>
	using VoidableRef_t = typename VoidableRef<T>::Type;
}