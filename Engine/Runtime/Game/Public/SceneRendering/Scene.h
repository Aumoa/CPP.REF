// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

interface ID3D12GraphicsCommandList4;
class Engine;
class PrimitiveSceneProxy;
class GPrimitiveComponent;
class APlayerController;
class SceneVisibility;
class GLightComponent;
class LightSceneProxy;
class DirectXAccelerationInstancingScene;
class DirectXShaderBindingTable;
class DirectXDescriptorAllocator;

class Scene : virtual public Object
{
public:
	using Super = Object;

private:
	Engine* engine;

	std::vector<GPrimitiveComponent*> primitiveComponents;
	std::vector<PrimitiveSceneProxy*> sceneProxies;

	APlayerController* localPlayer;
	TRefPtr<SceneVisibility> localPlayerVisibility;

	size_t numSRVs;
	std::vector<GLightComponent*> lightComponents;
	std::vector<LightSceneProxy*> lightProxies;

	TRefPtr<DirectXAccelerationInstancingScene> instancingScene;
	TRefPtr<DirectXShaderBindingTable> sbt;
	TRefPtr<DirectXDescriptorAllocator> allocator;

public:
	Scene(APlayerController* inPlayerController);
	~Scene() override;

	virtual void Update();
	virtual void CalcVisibility();

	virtual void BeginRender(ID3D12GraphicsCommandList4* inCommandList);
	virtual void EndRender(ID3D12GraphicsCommandList4* inCommandList);

	virtual void AddPrimitive(GPrimitiveComponent* inPrimitiveComponent);
	virtual void AddLight(GLightComponent* inLightComponent);

	virtual SceneVisibility* GetLocalPlayerVisibility() const;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const;
	virtual std::span<LightSceneProxy* const> GetLights() const;

	Engine* GetEngine() const;
	DirectXAccelerationInstancingScene* GetAccelScene() const;
	DirectXShaderBindingTable* GetShaderBindingTable() const;
	DirectXDescriptorAllocator* GetDescriptorAllocator() const;
};