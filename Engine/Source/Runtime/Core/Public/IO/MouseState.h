// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "MouseButton.h"
#include "Numerics/VectorInterface/Vector.h"
#include <array>

struct MouseState
{
	std::array<bool, (int32)EMouseButton::Max> ButtonState;
	Vector2N CursorLocation;
};