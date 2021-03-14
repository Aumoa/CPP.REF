// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface IRHICommandList;
class GPrimitiveComponent;
class GLightComponent;
class PrimitiveSceneProxy;
class LightSceneProxy;
class SceneVisibility;

interface GAME_API IRHIScene : virtual public Object
{
	virtual void Update() = 0;
	virtual void CalcVisibility() = 0;

	virtual void BeginRender(IRHICommandList* inCommandList) = 0;
	virtual void EndRender(IRHICommandList* inCommandList) = 0;

	virtual void AddPrimitive(GPrimitiveComponent* inPrimitiveComponent) = 0;
	virtual void AddLight(GLightComponent* inLightComponent) = 0;

	virtual SceneVisibility* GetLocalPlayerVisibility() const = 0;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const = 0;
	virtual std::span<LightSceneProxy* const> GetLights() const = 0;
};