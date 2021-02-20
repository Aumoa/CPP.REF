// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "VulkanResource.h"

VulkanResource::VulkanResource(VkImage vkImage, bool bDynamic) : Super()
	, bDynamic(bDynamic)
	, vkImage(vkImage)
{
	if (bDynamic)
	{
		throw;
	}
}

VulkanResource::~VulkanResource()
{

}

VkImage VulkanResource::Image_get() const
{
	return vkImage;
}