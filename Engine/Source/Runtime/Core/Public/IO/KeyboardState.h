// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Key.h"
#include <array>

struct KeyboardState
{
	std::array<bool, (int32)EKey::Max> Keyboard;
};