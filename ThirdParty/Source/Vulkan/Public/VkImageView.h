// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class SVkImage;

class VULKAN_API SVkImageView : public SVkDeviceChild, implements IRHIRenderTargetView
{
	GENERATED_BODY(SVkImageView)

private:
	VkDevice vkDevice = VK_NULL_HANDLE;

	SPROPERTY(Images)
	std::vector<SVkImage*> Images;
	std::vector<VkImageView> vkImageViews;

public:
	SVkImageView(SVkDevice* Owner, size_t NumViews);

	using Super::Dispose;

	virtual int32 GetViewCount() override;
	virtual IRHIResource* GetResource(int32 IndexOf) override;

	virtual void CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc) override;

	VkImage GetVkImage(size_t IndexOf);
	VkImageView GetVkImageView(size_t IndexOf);

protected:
	virtual void Dispose(bool bDisposing) override;
};