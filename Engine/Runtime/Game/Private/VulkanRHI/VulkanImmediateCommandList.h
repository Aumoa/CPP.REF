// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "VulkanCommandList.h"
#include "RHI/IRHIImmediateCommandList.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::RHI
{
	interface IRHIDeferredCommandList;
}

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanImmediateCommandList : public VulkanCommandList, virtual public RHI::IRHIImmediateCommandList
	{
	public:
		using Super = VulkanCommandList;
		using This = VulkanImmediateCommandList;

	private:

	public:
		VulkanImmediateCommandList();
		~VulkanImmediateCommandList() override;

		virtual void ExecuteCommandList(RHI::IRHIDeferredCommandList* deferredCommandList);
		virtual void Flush();
	};
}