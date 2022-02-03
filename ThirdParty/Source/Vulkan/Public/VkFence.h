// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkFence : public SVkDeviceChild, implements IRHIFence
{
	GENERATED_BODY(SVkFence)

public:
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkFence vkFence = VK_NULL_HANDLE;
		
public:
	SVkFence(SVkDevice* Owner, VkFence vkFence);

	using Super::Dispose;

	virtual void Wait() override;
	virtual bool IsReady() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};