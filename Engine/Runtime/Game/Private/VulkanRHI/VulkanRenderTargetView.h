// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIRenderTargetView.h"

#include "VulkanMinimal.h"

class VulkanRenderTargetView : virtual public Object, virtual public IRHIRenderTargetView
{
public:
	using Super = Object;
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