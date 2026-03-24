// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	template<size_t N, class T = float>
	struct DefaultVectors
	{
	};

	template<class T>
	struct DefaultVectors<3, T>
	{
		static constexpr Vector3<T> Forward = Vector3<T>(0, 0, 1);
		static constexpr Vector3<T> Up = Vector3<T>(0, 1, 0);
		static constexpr Vector3<T> Right = Vector3<T>(1, 0, 0);
	};

	template<class T>
	struct DefaultVectors<2, T>
	{
		static constexpr Vector2<T> Up = Vector2<T>(0, 1);
		static constexpr Vector2<T> Right = Vector2<T>(1, 0);
	};
}