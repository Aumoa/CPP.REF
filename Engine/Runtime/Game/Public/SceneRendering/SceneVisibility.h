// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/MinimalViewInfo.h"

class PrimitiveSceneProxy;
class Scene;

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	Scene* myScene;
	MinimalViewInfo myView;
	std::vector<bool> visibilities;

public:
	SceneVisibility(Scene* inScene, MinimalViewInfo& inView);
	SceneVisibility(const SceneVisibility& rh);
	SceneVisibility(SceneVisibility&& rh) noexcept;
	~SceneVisibility() override;

	void CalcVisibility();

	vs_property_get(const std::vector<bool>&, PrimitiveVisibility);
	const std::vector<bool>& PrimitiveVisibility_get() const;
};