// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "PaintArgs.h"

using namespace std;
using namespace std::chrono;

PaintArgs::PaintArgs(const SWidget* parentWidget, duration<float> deltaTime) : Super()
, _parent(parentWidget)
, _deltaTime(deltaTime)
{
}

PaintArgs* PaintArgs::WithNewParent(const SWidget* parentWidget) const
{
	return GetOuter()->CreateSubobject<PaintArgs>(parentWidget, _deltaTime);
}