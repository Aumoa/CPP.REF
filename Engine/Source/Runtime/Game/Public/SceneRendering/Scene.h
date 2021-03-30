
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
class DirectXDeviceContext;
class DirectXInstancedBufferAllocator;
class DirectXShaderResourceView;
class ShaderManager;

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

	TRefPtr<DirectXShaderResourceView> sceneSRV;
	TRefPtr<DirectXAccelerationInstancingScene> instancingScene;
	TRefPtr<DirectXShaderBindingTable> sbt;
	TRefPtr<DirectXDescriptorAllocator> allocator;
	TRefPtr<DirectXInstancedBufferAllocator> lightInstanced;

	TRefPtr<ShaderManager> shaderManager;

public:
	Scene(APlayerController* inPlayerController);
	~Scene() override;

	virtual void Update();
	virtual void InitViews();

	virtual void BeginRender(DirectXDeviceContext* deviceContext);
	virtual void EndRender(DirectXDeviceContext* deviceContext);

	virtual void AddPrimitive(GPrimitiveComponent* inPrimitiveComponent);
	virtual void AddLight(GLightComponent* inLightComponent);

	virtual SceneVisibility* GetLocalPlayerVisibility() const;
	virtual std::span<PrimitiveSceneProxy* const> GetPrimitives() const;
	virtual std::span<LightSceneProxy* const> GetLights() const;

	Engine* GetEngine() const;
	vs_property_get_auto(DirectXShaderResourceView*, SceneSRV, sceneSRV.Get());
	vs_property_get_auto(DirectXShaderBindingTable*, ShaderBindingTable, sbt.Get());
	vs_property_get_auto(DirectXDescriptorAllocator*, DescriptorAllocator, allocator.Get());

private:
	void UpdateInstancingBufferSRV();
	void UpdateLightInstancedSRV();
};