// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkDeviceChild.h"

class VULKAN_API SVkRenderPass : public SVkDeviceChild, implements IRHIRenderPass
{
	GENERATED_BODY(SVkRenderPass)

private:
	VkDevice vkDevice = VK_NULL_HANDLE;

	std::vector<VkAttachmentDescription> vkAttachments;
	std::vector<VkAttachmentReference> vkColorAttachments;
	bool bRecreateRenderPass = false;

public:
	VkRenderPass vkRenderPass = VK_NULL_HANDLE;
	VkFramebuffer vkFramebuffer = VK_NULL_HANDLE;

public:
	SVkRenderPass(SVkDevice* Owner);

	using Super::Dispose;

	virtual void AddColorAttachment(const RHIRenderPassAttachment& AttachDesc) override;
	virtual void Apply(std::span<std::pair<IRHIView*, int32> const> AttachViews, const Vector2N& Size) override;

protected:
	virtual void Dispose(bool bDisposing) override;
};