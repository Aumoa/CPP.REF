// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "Diagnostics/ScopedCycleCounter.h"
#include "SceneRendering/MinimalViewInfo.h"

interface IRHIScene;
class PrimitiveSceneProxy;
class ShaderCameraConstantVector;

class GAME_API SceneVisibility : virtual public Object
{
public:
	using Super = Object;
	using This = SceneVisibility;

private:
	IRHIScene* myScene;
	MinimalViewInfo myView;
	std::vector<bool> visibilities;
	TRefPtr<ShaderCameraConstantVector> shaderCameraConstants;
	bool bDirty : 1;
	Frustum viewFrustum;
	size_t numPrimitivesRender;

public:
	SceneVisibility(IRHIScene* inScene);
	~SceneVisibility() override;

	void CalcVisibility();
	void UpdateView(const MinimalViewInfo& inView);

	vs_property_get(const std::vector<bool>&, PrimitiveVisibility);
	vs_property_get(ShaderCameraConstantVector*, ShaderCameraConstants);
	vs_property_get_auto(size_t, NumPrimitivesRender, numPrimitivesRender);
};