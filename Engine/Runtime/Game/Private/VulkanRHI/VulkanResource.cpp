// Copyright 2020 Aumoa.lib. All right reserved.

#include "VulkanResource.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::VulkanRHI;

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