// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkFence.h"
#include "VkDevice.h"

GENERATE_BODY(SVkFence);

SVkFence::SVkFence(SVkDevice* Owner, VkFence vkFence)
	: Super(Owner)
	, vkFence(std::move(vkFence))
{
	vkDevice = Owner->vkDevice;
}

void SVkFence::Wait()
{
	VR(vkWaitForFences(vkDevice, 1, &vkFence, VK_TRUE, INFINITE));
}

bool SVkFence::IsReady()
{
	return vkGetFenceStatus(vkDevice, vkFence) != VK_NOT_READY;
}

void SVkFence::Dispose(bool bDisposing)
{
	if (vkFence != VK_NULL_HANDLE)
	{
		vkDestroyFence(vkDevice, vkFence, nullptr);
		vkFence = VK_NULL_HANDLE;
	}

	Super::Dispose(bDisposing);
}