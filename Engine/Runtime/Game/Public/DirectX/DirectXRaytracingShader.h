// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class GAME_API DirectXRaytracingShader : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;
	TComPtr<ID3D12RootSignature> rootSignature;
	TComPtr<ID3D12StateObject> pipelineState;
	TComPtr<ID3D12StateObjectProperties> properties;
	bool bInit : 1;

	std::vector<D3D12_HIT_GROUP_DESC> hitGroups;
	std::vector<TComPtr<ID3D12RootSignature>> localRS;

public:
	DirectXRaytracingShader(DirectXDeviceBundle* deviceBundle);
	~DirectXRaytracingShader() override;

	void Init();
	void Render(ID3D12GraphicsCommandList4* inCommandList);

	const void* GetRayGenerationIdentifier() const;
	const void* GetClosestHitIdentifier(size_t shaderIndex) const;
	const void* GetMissIdentifier(size_t shaderIndex) const;

	vs_property_get_auto(bool, IsInitialized, bInit);
	vs_property_get_auto(ID3D12RootSignature*, RootSignature, rootSignature.Get());
	vs_property_get_auto(ID3D12StateObject*, PipelineState, pipelineState.Get());

private:
	void InitRS();
	void InitLocalRS();
	void InitHitGroups();
	void InitPS();
};