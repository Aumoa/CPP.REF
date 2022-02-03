// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class SVkCommandAllocator : public SVkDeviceChild, implements IRHICommandAllocator
{
	GENERATED_BODY(SVkCommandAllocator)

public:
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkCommandPool vkCommandPool = VK_NULL_HANDLE;

public:
	SVkCommandAllocator(SVkDevice* Owner, VkCommandPool vkCommandPool);

	using Super::Dispose;

	virtual void Reset() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};