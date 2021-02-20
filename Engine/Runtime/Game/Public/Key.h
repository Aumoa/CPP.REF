// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

enum class EKey
{
	LeftButton = 1, RightButton, MiddleButton = 4,
	ESC = 27,
	A = 65, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
};

enum class EKeyEvent
{
	Pressed,
	Released,
};