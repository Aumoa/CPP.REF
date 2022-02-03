// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkFactoryChild.h"

class VULKAN_API SVkAdapter : public SVkFactoryChild, implements IRHIAdapter
{
	GENERATED_BODY(SVkAdapter)

private:
	VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
	std::wstring DeviceName;

public:
	SVkAdapter(SVkFactory* Owner, VkPhysicalDevice vkPhysicalDevice);

	using Super::Dispose;

	virtual std::wstring GetDeviceName() override;

	VkPhysicalDevice GetPhysicalDevice() { return vkPhysicalDevice; }

protected:
	virtual void Dispose(bool bDisposing) override;
};