﻿// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIScene.h"

#include "DirectX/DirectXMinimal.h"

class PrimitiveSceneProxy;
class GPrimitiveComponent;
class APlayerController;
class SceneVisibility;
class GLightComponent;
class LightSceneProxy;
class D3D12DeviceBundle;
class D3D12ResourceBundle;

class Scene : virtual public Object, virtual public IRHIScene
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

public:
	Scene(APlayerController* inPlayerController);
	~Scene() override;

	virtual void Update();
	virtual void CalcVisibility();

	virtual void BeginRender(IRHICommandList* inCommandList);
	virtual void EndRender(IRHICommandList* inCommandList);

	virtual void AddPrimitive(GPrimitiveComponent* inPrimitiveComponent);
	virtual void AddLight(GLightComponent* inLightComponent);

	virtual SceneVisibility* GetLocalPlayerVisibility() const;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const;
	virtual std::span<LightSceneProxy* const> GetLights() const;
};