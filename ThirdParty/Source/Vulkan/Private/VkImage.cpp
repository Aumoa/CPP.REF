// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkImage.h"
#include "VkDevice.h"

GENERATE_BODY(SVkImage);

SVkImage::SVkImage(SVkDevice* Owner, VkImage vkImage, const Vector2N& Size, bool bDisposable)
	: Super(Owner)
	, bDisposable(bDisposable)
	, Size(Size)
	, vkImage(std::move(vkImage))
{
	vkDevice = Owner->vkDevice;
}

Vector2N SVkImage::GetPixelSize()
{
	return Size;
}

void SVkImage::Dispose(bool bDisposing)
{
	if (bDisposable)
	{
		if (vkImage != VK_NULL_HANDLE)
		{
			vkDestroyImage(vkDevice, vkImage, nullptr);
			vkImage = VK_NULL_HANDLE;
		}
	}

	Super::Dispose(bDisposing);
}