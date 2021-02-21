// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShaderBindingTable.h"

#include "D3D12Minimal.h"

class D3D12Shader;

struct D3D12ShaderBindingParam
{
	char Identifier[D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES];
	D3D12_GPU_DESCRIPTOR_HANDLE RootParameters[1];
};

class D3D12SBTAllocator : virtual public Object, virtual public IRHIShaderBindingTable
{
public:
	using Super = Object;

private:
	ComPtr<ID3D12Resource> rayGeneration;

public:
	D3D12SBTAllocator(ID3D12Device5* device, D3D12Shader* inShader);
	~D3D12SBTAllocator() override;

	virtual uint64 RayGenerationStartAddress_get() const;
	virtual uint64 RayGenerationBufferStride_get() const;
};