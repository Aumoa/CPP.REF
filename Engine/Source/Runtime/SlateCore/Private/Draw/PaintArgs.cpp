// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Draw/PaintArgs.h"

SPaintArgs::SPaintArgs(const SWidget* parentWidget, std::chrono::duration<float> deltaTime) : Super()
	, _parent(parentWidget)
	, _deltaTime(deltaTime)
{
}

SPaintArgs* SPaintArgs::WithNewParent(const SWidget* parentWidget) const
{
	return GetOuter()->NewObject<SPaintArgs>(parentWidget, _deltaTime);
}