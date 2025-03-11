// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "GameObject.h"

class ENGINE_API GameInstance
{
private:
	String GameName;

public:
	GameInstance(String InGameName);

	String GetName() const { return GameName; }
};