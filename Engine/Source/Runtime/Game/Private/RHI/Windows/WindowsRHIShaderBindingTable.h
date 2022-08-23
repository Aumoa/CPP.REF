// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIShaderBindingTable.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHIShaderBindingTable : public RHIShaderBindingTable
{
	friend WindowsRHIDevice;

private:
	struct ShaderParameterTemplate
	{
		char ShaderIdentifier[D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES];
		uint64 Parameters[1];
	};

private:
	std::shared_ptr<RHIResource> _bindingResource;
	ShaderParameterTemplate* _mapped = nullptr;
	ID3D12Resource* _ptr = nullptr;

private:
	WindowsRHIShaderBindingTable(std::shared_ptr<WindowsRHIDevice> device);

public:
	virtual ~WindowsRHIShaderBindingTable() noexcept override;

	virtual RHIGPUVirtualAddressRange GetRayGenerationShaderRecord(const void* shaderIdentifier) const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetMissShaderTable() const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetHitGroupTable() const override;
	virtual RHIGPUVirtualAddressRangeAndStride GetCallableShaderTable() const override;
};

#endif