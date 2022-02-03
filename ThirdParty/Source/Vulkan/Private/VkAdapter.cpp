// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkAdapter.h"

GENERATE_BODY(SVkAdapter);

SVkAdapter::SVkAdapter(SVkFactory* Owner, VkPhysicalDevice vkPhysicalDevice)
	: Super(Owner)
	, vkPhysicalDevice(std::move(vkPhysicalDevice))
{
	VkPhysicalDeviceProperties vkProperties = {};
	vkGetPhysicalDeviceProperties(this->vkPhysicalDevice, &vkProperties);
	DeviceName = ANSI_TO_WCHAR(vkProperties.deviceName);
}

std::wstring SVkAdapter::GetDeviceName()
{
	return DeviceName;
}

void SVkAdapter::Dispose(bool bDisposing)
{
	if (vkPhysicalDevice != VK_NULL_HANDLE)
	{
		vkPhysicalDevice = VK_NULL_HANDLE;
	}

	Super::Dispose(bDisposing);
}