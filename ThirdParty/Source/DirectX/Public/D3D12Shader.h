// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIShader.h"

class DIRECTX_API SD3D12Shader : public SD3D12DeviceChild, implements IRHIShader
{
	GENERATED_BODY(SD3D12Shader)

private:
	ComPtr<ID3D12RootSignature> _rootSignature;
	ComPtr<ID3D12PipelineState> _pipelineState;

public:
	SD3D12Shader(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12PipelineState> pipelineState);

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12RootSignature, _rootSignature);
	DECLARE_GETTER(ID3D12PipelineState, _pipelineState);
};