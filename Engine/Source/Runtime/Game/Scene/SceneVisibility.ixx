// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:SceneVisibility;

import std.core;
import SC.Runtime.Core;
import SC.Runtime.RenderCore;
import :MinimalViewInfo;

export class Scene;

using namespace std;
using namespace std::chrono;

export class SceneVisibility : virtual public Object
{
public:
	using Super = Object;

private:
	Scene* _owner = nullptr;
	RHIResource* _viewBuffer = nullptr;
	size_t _viewBufCapa = 0;

public:
	SceneVisibility(Scene* owner);

	void CalcVisibility(duration<float> elapsedTime, const MinimalViewInfo& view);

private:
	void FrustumCull();
	void ReadyBuffer(size_t capa, bool bAllowShrink);
};