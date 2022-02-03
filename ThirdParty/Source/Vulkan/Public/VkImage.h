// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkImage : public SVkDeviceChild, implements IRHITexture2D
{
	GENERATED_BODY(SVkImage)

private:
	bool bDisposable = false;
	Vector2N Size;

public:
	VkDevice vkDevice = VK_NULL_HANDLE;
	VkImage vkImage = VK_NULL_HANDLE;

public:
	SVkImage(SVkDevice* Owner, VkImage vkImage, const Vector2N& Size, bool bDisposable);

	using Super::Dispose;

	virtual Vector2N GetPixelSize() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};