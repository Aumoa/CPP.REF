// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Draw/PaintArgs.h"

PaintArgs::PaintArgs(const SWidget* parentWidget, std::chrono::duration<float> deltaTime) : Super()
	, _parent(parentWidget)
	, _deltaTime(deltaTime)
{
}

PaintArgs* PaintArgs::WithNewParent(const SWidget* parentWidget) const
{
	return GetOuter()->NewObject<PaintArgs>(parentWidget, _deltaTime);
}