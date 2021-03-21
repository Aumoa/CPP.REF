// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

class PrimitiveSceneProxy;
class DirectXDeviceContext;

class GAME_API DirectXAccelerationInstancingScene : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;
	std::vector<const PrimitiveSceneProxy*> proxies;

	TComPtr<ID3D12Resource> accelerationStructure;
	TComPtr<ID3D12Resource> oldAccelerationStructure;
	TComPtr<ID3D12Resource> scratchBuffer;
	size_t lastCount;

	TComPtr<ID3D12Resource> instanceBuffer;
	size_t actualInstanceCount;

	bool bNeedRebuild : 1;
	bool bAllowUpdate : 1;

public:
	DirectXAccelerationInstancingScene(DirectXDeviceBundle* deviceBundle);
	~DirectXAccelerationInstancingScene() override;

	void UpdateScene();
	void BuildScene(DirectXDeviceContext* deviceContext);

	void AddInstance(size_t index, const PrimitiveSceneProxy* instanceDesc);
	void RemoveInstance(size_t index);

	void AddInstanceDeferred(size_t index, const PrimitiveSceneProxy* instanceDesc);
	void RemoveInstanceDeferred(size_t index);
	void FinishEditInstance();

	uint64 GetRaytracingAccelerationStructureBuffer() const;

private:
	void CheckAndReallocate(size_t desiredCount);
	D3D12_RAYTRACING_INSTANCE_DESC GetRaytracingInstanceDesc(uint32& shaderRecords, const PrimitiveSceneProxy* sceneProxy) const;
};