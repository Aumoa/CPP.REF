// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShader.h"

#include "D3D12Minimal.h"

class D3D12Shader : virtual public Object, virtual public IRHIShader
{
public:
	using Super = Object;
	using This = D3D12Shader;

private:
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12Pageable> pipelineState;

	bool bRaytracing : 1;
	size_t shaderTypeHash;

public:
	D3D12Shader(ID3D12RootSignature* inRootSignature, ID3D12PipelineState* inPipelineState);
	D3D12Shader(ID3D12RootSignature* inRootSignature, ID3D12StateObject* inPipelineState, ID3D12StateObjectProperties* raytracingProperties);
	~D3D12Shader();

	void InputShader(ID3D12GraphicsCommandList4* inCommandList);

	vs_property_get_auto(bool, IsRaytracingShader, bRaytracing);
};