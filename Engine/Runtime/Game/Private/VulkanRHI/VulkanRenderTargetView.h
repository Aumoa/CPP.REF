// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIRenderTargetView.h"

#include "VulkanMinimal.h"

namespace SC::Runtime::Game::VulkanRHI
{
	class VulkanRenderTargetView : virtual public Core::Object, virtual public RHI::IRHIRenderTargetView
	{
	public:
		using Super = Core::Object;
		using This = VulkanRenderTargetView;

	private:
		VkDevice vkDevice;
		VkImageView vkImageView;

	public:
		VulkanRenderTargetView(VkDevice vkDevice, VkImageView vkImageView);
		~VulkanRenderTargetView() override;

		vs_property_get(VkImageView, View);
		VkImageView View_get() const;
	};
}