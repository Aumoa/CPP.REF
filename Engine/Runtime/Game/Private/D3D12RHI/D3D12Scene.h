// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIScene.h"

#include "D3D12Minimal.h"

interface IRHIOnlineDescriptorPatch;
interface IRHICommandList;
interface IRHIResource;

class PrimitiveSceneProxy;
class GPrimitiveComponent;
class APlayerController;
class SceneVisibility;
class GLightComponent;
class LightSceneProxy;
class D3D12DeviceBundle;
class D3D12ResourceBundle;

class D3D12Scene : virtual public Object, virtual public IRHIScene
{
public:
	using Super = Object;

public:
	D3D12DeviceBundle* deviceBundle;
	D3D12ResourceBundle* resourceBundle;

	std::vector<GPrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

	APlayerController* localPlayer;
	TRefPtr<SceneVisibility> localPlayerVisibility;
	std::vector<TRefPtr<SceneVisibility>> visibilities;

	size_t numSRVs;
	std::vector<GLightComponent*> lightComponents;
	std::vector<LightSceneProxy*> lightProxies;
	TRefPtr<IRHIOnlineDescriptorPatch> srvPatch;

	TRefPtr<IRHIResource> topLevelAS;
	TRefPtr<IRHIResource> topLevelScratch;

public:
	D3D12Scene(APlayerController* inPlayerController);
	~D3D12Scene() override;

	virtual void Update();
	virtual void CalcVisibility();

	virtual void BeginRender(IRHICommandList* inCommandList);
	virtual void EndRender(IRHICommandList* inCommandList);

	virtual void AddPrimitive(GPrimitiveComponent* inPrimitiveComponent);
	virtual void AddLight(GLightComponent* inLightComponent);

	virtual SceneVisibility* GetLocalPlayerVisibility() const;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const;
	virtual std::span<LightSceneProxy* const> GetLights() const;

private:
	void ReadyRaytracingAccelerationBuffers();
};