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

	private:
		std::vector<D3D12_RESOURCE_BARRIER> pendingBarriers;

	public:
		D3D12CommandList();
		~D3D12CommandList() override;

		virtual void EndCommand();

		virtual void OMSetRenderTargets(size_t count, RHI::IRHIRenderTargetView* rtv[]);
		virtual void ClearRenderTargetView(RHI::IRHIRenderTargetView* rtv);
		virtual void ResourceTransition(RHI::IRHIResource* resource, RHI::RHIResourceStates beforeState, RHI::RHIResourceStates afterState, size_t subresourceIndex = 0);

		vs_property_get(ID3D12GraphicsCommandList*, CommandList);
		virtual ID3D12GraphicsCommandList* CommandList_get() = 0;

	private:
		void ConsumePendingDeferredCommands();
	};
}