// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAME_API GameThreads
{
private:
	static Thread _gameThread;

public:
	static void InitializeGameThread();
	static bool IsInGameThread();
};