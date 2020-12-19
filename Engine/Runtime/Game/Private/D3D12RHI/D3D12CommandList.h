// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandList.h"

#include "D3D12Minimal.h"

namespace SC::Runtime::Game::D3D12RHI
{
	class D3D12CommandList : virtual public Core::Object, virtual public RHI::IRHICommandList
	{
	public:
		using Super = Core::Object;

	public:
		D3D12CommandList();
		~D3D12CommandList() override;

		void OMSetRenderTargets(size_t count, RHI::IRHIRenderTargetView* rtv[]);

		vs_property_get(ID3D12GraphicsCommandList*, CommandList);
		virtual ID3D12GraphicsCommandList* CommandList_get() = 0;
	};
}