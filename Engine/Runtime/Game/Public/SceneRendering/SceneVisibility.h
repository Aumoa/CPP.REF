// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/MinimalViewInfo.h"

class Scene;
class PrimitiveSceneProxy;

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	Scene* myScene;
	MinimalViewInfo myView;
	std::vector<bool> visibilities;
	bool bDirty : 1;
	Frustum viewFrustum;
	size_t numPrimitivesRender;

public:
	SceneVisibility(Scene* inScene);
	~SceneVisibility() override;

	void CalcVisibility();
	void UpdateView(const MinimalViewInfo& inView);

	vs_property_get(const std::vector<bool>&, PrimitiveVisibility);
	vs_property_get_auto(size_t, NumPrimitivesRender, numPrimitivesRender);
};