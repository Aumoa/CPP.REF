// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHIRootSignature.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/RHIStructures.h"

std::shared_ptr<RHIRootSignature> WindowsRHIDevice::CreateRootSignature(const RHIRootSignatureDesc& desc)
{
	return std::shared_ptr<RHIRootSignature>(new WindowsRHIRootSignature(SharedFromThis(), desc));
}

WindowsRHIRootSignature::WindowsRHIRootSignature(std::shared_ptr<WindowsRHIDevice> device, const RHIRootSignatureDesc& desc)
	: RHIRootSignature(device)
{
	WindowsConvertMemoryHeap convertHeap;
	D3D12_ROOT_SIGNATURE_DESC d3ddesc = Convert(desc, convertHeap);

	auto* d3ddev = device->GetDevice();

	ComPtr<ID3DBlob> blob, error;
	HRESULT hCode = D3D12SerializeRootSignature(&d3ddesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &blob, &error);
	if (FAILED(hCode))
	{
		if (error)
		{
			checkf(SUCCEEDED(hCode) && "D3D12SerializeRootSignature()", TEXT("DirectX RHI assertion failure. HRESULT: {:X}, Message: {}"), (int32)hCode, String((const char*)error->GetBufferPointer(), error->GetBufferSize()));
		}
		else
		{
			HR(SUCCEEDED(hCode) && "D3D12SerializeRootSignature()");
		}
	}

	HR(d3ddev->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&_signature)));
}

WindowsRHIRootSignature::~WindowsRHIRootSignature() noexcept
{
}

#endif