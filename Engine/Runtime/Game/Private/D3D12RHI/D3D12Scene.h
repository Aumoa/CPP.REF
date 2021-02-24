// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIScene.h"

#include "D3D12Minimal.h"

interface IRHIOnlineDescriptorPatch;
interface IRHICommandList;

class PrimitiveSceneProxy;
class PrimitiveComponent;
class APlayerController;
class SceneVisibility;
class LightComponent;
class LightSceneProxy;

class D3D12Scene : virtual public Object, virtual public IRHIScene
{
public:
	using Super = Object;

public:
	std::vector<PrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

	APlayerController* localPlayer;
	TRefPtr<SceneVisibility> localPlayerVisibility;
	std::vector<TRefPtr<SceneVisibility>> visibilities;

	size_t numSRVs;
	std::vector<LightComponent*> lightComponents;
	std::vector<LightSceneProxy*> lightProxies;
	TRefPtr<IRHIOnlineDescriptorPatch> srvPatch;

public:
	D3D12Scene(APlayerController* inPlayerController);
	~D3D12Scene() override;

	virtual void Update();
	virtual void CalcVisibility();

	virtual void BeginRender(IRHICommandList* inCommandList);
	virtual void EndRender(IRHICommandList* inCommandList);

	virtual void AddPrimitive(PrimitiveComponent* inPrimitiveComponent);
	virtual void AddLight(LightComponent* inLightComponent);

	virtual SceneVisibility* GetLocalPlayerVisibility() const;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const;
	virtual std::span<LightSceneProxy* const> GetLights() const;
};