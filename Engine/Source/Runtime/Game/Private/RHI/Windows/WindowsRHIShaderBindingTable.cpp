// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIShaderBindingTable.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIResource.h"
#include "RHI/RHIStructures.h"

std::shared_ptr<RHIShaderBindingTable> WindowsRHIDevice::CreateShaderBindingTable()
{
	return MakeShared<WindowsRHIShaderBindingTable>(SharedFromThis());
}

WindowsRHIShaderBindingTable::WindowsRHIShaderBindingTable(std::shared_ptr<WindowsRHIDevice> device)
	: RHIShaderBindingTable(device)
{
	_bindingResource = device->CreateCommittedBuffer(ERHIHeapType::Upload, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES, ERHIResourceStates::GenericRead, ERHIResourceFlags::None);

	_ptr = WinGetr(_bindingResource.get());
	HR(_ptr->Map(0, nullptr, (void**)& _mapped));
}

WindowsRHIShaderBindingTable::~WindowsRHIShaderBindingTable() noexcept
{
}

RHIGPUVirtualAddressRange WindowsRHIShaderBindingTable::GetRayGenerationShaderRecord(const void* shaderIdentifier) const
{
	memcpy(_mapped->ShaderIdentifier, shaderIdentifier, SE_ARRAYSIZE(_mapped->ShaderIdentifier));
	return { .StartAddress = _ptr->GetGPUVirtualAddress(), .SizeInBytes = SE_ARRAYSIZE(_mapped->ShaderIdentifier) };
}

RHIGPUVirtualAddressRangeAndStride WindowsRHIShaderBindingTable::GetMissShaderTable() const
{
	return {};
}

RHIGPUVirtualAddressRangeAndStride WindowsRHIShaderBindingTable::GetHitGroupTable() const
{
	return {};
}

RHIGPUVirtualAddressRangeAndStride WindowsRHIShaderBindingTable::GetCallableShaderTable() const
{
	return {};
}

#endif