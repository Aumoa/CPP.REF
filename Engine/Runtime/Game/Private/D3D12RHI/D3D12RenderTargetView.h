// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIRenderTargetView.h"

#include "D3D12Minimal.h"
#include "D3D12DescriptorIndex.h"

namespace SC::Runtime::Game::D3D12RHI
{
	struct D3D12DescriptorIndex;

	class D3D12RenderTargetView : virtual public Core::Object, virtual public RHI::IRHIRenderTargetView
	{
	public:
		using Super = Core::Object;
		using This = D3D12RenderTargetView;

	private:
		D3D12DescriptorIndex handle;

	public:
		D3D12RenderTargetView(ID3D12Resource* resource, const D3D12DescriptorIndex& handle);
		~D3D12RenderTargetView() override;

		vs_property_get(D3D12_CPU_DESCRIPTOR_HANDLE, Handle);
		D3D12_CPU_DESCRIPTOR_HANDLE Handle_get() const;
	};
}