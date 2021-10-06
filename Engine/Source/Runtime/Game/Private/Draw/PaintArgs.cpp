// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Draw/PaintArgs.h"

SPaintArgs::SPaintArgs(SWidget* parentWidget, std::chrono::duration<float> deltaTime) : Super()
	, _parent(parentWidget)
	, _deltaTime(deltaTime)
{
}

SPaintArgs* SPaintArgs::WithNewParent(SWidget* parentWidget)
{
	return GetOuter()->NewObject<SPaintArgs>(parentWidget, _deltaTime);
}