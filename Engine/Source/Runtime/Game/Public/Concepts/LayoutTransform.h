// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

template<class T>
concept LayoutTransform = requires(T transform)
{
	{ transform.TransformPoint(std::declval<Vector2>()) } -> std::same_as<Vector2>;
	{ transform.TransformVector(std::declval<Vector2>()) } -> std::same_as<Vector2>;
	{ transform.Inverse() };
};