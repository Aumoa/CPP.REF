// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;

class GAME_API SPaintArgs : implements SObject
{
	GENERATED_BODY(SPaintArgs)

private:
	SWidget* _parent = nullptr;
	std::chrono::duration<float> _deltaTime;

public:
	SPaintArgs(SWidget* parentWidget, std::chrono::duration<float> deltaTime);

	SPaintArgs* WithNewParent(SWidget* parentWidget);

	SWidget* GetParentWidget() { return _parent; }
	std::chrono::duration<float> GetDeltaTime() { return _deltaTime; }
};