// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkImageView.h"
#include "VkDevice.h"
#include "VkImage.h"

GENERATE_BODY(SVkImageView);

SVkImageView::SVkImageView(SVkDevice* Owner, size_t NumViews)
	: Super(Owner)
	, vkDevice(Owner->vkDevice)
{
	Images.resize(NumViews, VK_NULL_HANDLE);
	vkImageViews.resize(NumViews, VK_NULL_HANDLE);
}

int32 SVkImageView::GetViewCount()
{
	return (int32)Images.size();
}

IRHIResource* SVkImageView::GetResource(int32 IndexOf)
{
	return Images[IndexOf];
}

void SVkImageView::CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc)
{
	SVkImage*& Image = Images[Index];
	VkImageView& vkImageView = vkImageViews[Index];

	if (vkImageView != VK_NULL_HANDLE)
	{
		vkDestroyImageView(vkDevice, vkImageView, nullptr);
		vkImageView = VK_NULL_HANDLE;
		Image = nullptr;
	}

	if (auto* sImage = Cast<SVkImage>(Resource))
	{
		VkImageViewCreateInfo vkImageViewCreate =
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = sImage ? sImage->vkImage : VK_NULL_HANDLE,
			.viewType = AsVkImageViewType(Desc->ViewDimension),
			.format = AsVkFormat(Desc->Format),
			.components = VK_DEFAULT_COMPONTN_MAPPING(),
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.levelCount = 1,
				.layerCount = 1
			}
		};

		vkCreateImageView(vkDevice, &vkImageViewCreate, nullptr, &vkImageView);
		Image = sImage;
	}
}

VkImage SVkImageView::GetVkImage(size_t IndexOf)
{
	return Images[IndexOf]->vkImage;
}

VkImageView SVkImageView::GetVkImageView(size_t IndexOf)
{
	return vkImageViews[IndexOf];
}

void SVkImageView::Dispose(bool bDisposing)
{
	for (auto& vkImageView : vkImageViews)
	{
		if (vkImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(vkDevice, vkImageView, nullptr);
		}
	}

	vkImageViews.clear();
	Images.clear();

	Super::Dispose(bDisposing);
}