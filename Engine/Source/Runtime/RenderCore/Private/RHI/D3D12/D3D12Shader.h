// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIShader.h"
#include "RHI/DirectXCommon.h"

class SD3D12Shader : public SD3D12DeviceChild, implements IRHIShader
{
	GENERATED_BODY(SD3D12Shader)

private:
	ComPtr<ID3D12RootSignature> _rootSignature;
	ComPtr<ID3D12PipelineState> _pipelineState;

public:
	SD3D12Shader(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12PipelineState> pipelineState);

public:
	DECLARE_GETTER(ID3D12RootSignature, _rootSignature);
	DECLARE_GETTER(ID3D12PipelineState, _pipelineState);
};