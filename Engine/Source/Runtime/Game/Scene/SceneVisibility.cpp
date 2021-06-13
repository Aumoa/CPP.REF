// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;
import SC.Runtime.RenderCore;

using namespace std::chrono;

SceneVisibility::SceneVisibility(Scene* owner) : Super()
	, _owner(owner)
{
}

void SceneVisibility::CalcVisibility(duration<float> elapsedTime, const MinimalViewInfo& view)
{
}

void SceneVisibility::FrustumCull()
{
}

void SceneVisibility::ReadyBuffer(size_t capa, bool bAllowShrink)
{
	RHIDevice* dev = _owner->GetDevice();
	size_t prev = sizeof(RHIViewConstants) * _viewBufCapa;
	size_t next = sizeof(RHIViewConstants) * capa;

	if (prev >= next)
	{
		// The view buffer not be allowed to shrink. Return immediately.
		return;
	}

	DestroySubobject(_viewBuffer);
	_viewBuffer = dev->CreateDynamicBuffer(next);
}