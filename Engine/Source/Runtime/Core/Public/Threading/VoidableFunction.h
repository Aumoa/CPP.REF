// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <functional>

namespace Threading::Tasks
{
	template<class T>
	struct VoidableFunction_t
	{
		using Type = std::function<void(T)>;
	};

	template<>
	struct VoidableFunction_t<void>
	{
		using Type = std::function<void()>;
	};

	template<class T>
	using VoidableFunction = typename VoidableFunction_t<T>::Type;
}