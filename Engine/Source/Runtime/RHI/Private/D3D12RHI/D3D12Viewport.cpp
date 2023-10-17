// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Viewport.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Texture2D.h"
#include "D3D12RHI/D3D12ShaderResourceView.h"
#include "D3D12RHI/D3D12Global.h"

ND3D12Viewport::ND3D12Viewport(ID3D12Device* pInDevice)
	: pDevice(pInDevice)
{
	SRV = std::make_shared<ND3D12ShaderResourceView>(*pInDevice, 1);
}

ND3D12Viewport::~ND3D12Viewport() noexcept
{
}

Vector2N ND3D12Viewport::GetViewportSize() const
{
	return VpSize;
}

void ND3D12Viewport::Resize(const Vector2N& InSize)
{
	if (pResource)
	{
		// enqueue resource for delay release after all commands are completed.
		std::ignore = ND3D12Global::GetDynamicRHI().EnqueueGraphicsCommandAsync([pPendingKill = pResource](ID3D12GraphicsCommandList*)
		{
			return false;
		});
	}

	// create resource.
	D3D12_CLEAR_VALUE ClearValue{ DXGI_FORMAT_R8G8B8A8_UNORM };
	D3D12_RESOURCE_DESC Desc = GetTexture2DDesc(InSize, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, DXGI_FORMAT_R8G8B8A8_UNORM);
	HR(pDevice->CreateCommittedResource(&DefaultHeap, D3D12_HEAP_FLAG_NONE, &Desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &ClearValue, IID_PPV_ARGS(&pResource)));

	// create RTV.
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc =
	{
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D,
	};
	pDevice->CreateRenderTargetView(pResource.Get(), &RTVDesc, GetRTVHandle());

	// create SRV.
	ND3D12Texture2D StackView(pResource, Desc);
	SRV->CreateView(0, &StackView);
}

#endif