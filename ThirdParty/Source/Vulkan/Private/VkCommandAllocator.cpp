// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkCommandAllocator.h"
#include "VkDevice.h"

GENERATE_BODY(SVkCommandAllocator);

SVkCommandAllocator::SVkCommandAllocator(SVkDevice* Owner, VkCommandPool vkCommandPool)
	: Super(Owner)
	, vkCommandPool(std::move(vkCommandPool))
{
	vkDevice = Owner->vkDevice;
}

void SVkCommandAllocator::Reset()
{
	VR(vkResetCommandPool(vkDevice, vkCommandPool, 0));
}

void SVkCommandAllocator::Dispose(bool bDisposing)
{
	if (vkCommandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(vkDevice, vkCommandPool, nullptr);
		vkCommandPool = VK_NULL_HANDLE;
	}

	Super::Dispose(bDisposing);
}