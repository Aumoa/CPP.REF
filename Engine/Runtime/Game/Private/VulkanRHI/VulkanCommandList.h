// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHICommandList.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanCommandList : virtual public Core::Object, virtual public RHI::IRHICommandList
	{
	public:
		using Super = Core::Object;
		using This = VulkanCommandList;

	private:

	public:
		VulkanCommandList();
		~VulkanCommandList() override;

		virtual void BeginCommand();
		virtual void EndCommand();

		virtual void OMSetRenderTargets(size_t count, RHI::IRHIRenderTargetView* rtv[]);
		virtual void ClearRenderTargetView(RHI::IRHIRenderTargetView* rtv);
		virtual void ResourceTransition(RHI::IRHIResource* resource, RHI::RHIResourceStates beforeState, RHI::RHIResourceStates afterState, size_t subresourceIndex = 0);
		virtual void CopyResource(RHI::IRHIResource* target, RHI::IRHIResource* source);

		virtual bool HasBegunCommand_get() const;
	};
}