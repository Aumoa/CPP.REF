// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <functional>

namespace Ayla
{
	template<class T>
#if __cpp_lib_move_only_function
	using MoveOnlyFunction = std::move_only_function<T>;
#else
	using MoveOnlyFunction = std::function<T>;
#endif
}
