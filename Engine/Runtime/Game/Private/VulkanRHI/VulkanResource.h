// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"

#include "VulkanMinimal.h"

class VulkanResource : virtual public Object, virtual public IRHIResource
{
public:
	using Super = Object;
	using This = VulkanResource;

private:
	const bool bDynamic : 1;
	VkImage vkImage;

public:
	VulkanResource(VkImage vkImage, bool bDynamic);
	~VulkanResource() override;

	vs_property_get(VkImage, Image);
	VkImage Image_get() const;
};