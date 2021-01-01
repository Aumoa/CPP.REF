﻿// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShader.h"

#include <span>
#include "D3D12Minimal.h"

class D3D12Shader : virtual public Object, virtual public IRHIShader
{
public:
	using Super = Object;
	using This = D3D12Shader;

private:
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12PipelineState> pipelineState;

	std::vector<uint8> vertexShaderBytecode;
	std::vector<uint8> pixelShaderBytecode;

	bool bShaderInitialized : 1;
	size_t shaderTypeHash;

public:
	D3D12Shader();
	~D3D12Shader();

	size_t GetHashCode() const override;
	size_t GetShaderTypeHashCode() const override;

	void SetVertexShader(std::span<const uint8> shaderBytecode);
	void SetPixelShader(std::span<const uint8> shaderBytecode);

	void CreateShaderPipeline(ID3D12Device* device);

	vs_property_get(bool, ShaderInitialized);
	bool ShaderInitialize_get() const;

	vs_property_get(ID3D12RootSignature*, RootSignature);
	ID3D12RootSignature* RootSignature_get() const;
	vs_property_get(ID3D12PipelineState*, PipelineState);
	ID3D12PipelineState* PipelineState_get() const;
};