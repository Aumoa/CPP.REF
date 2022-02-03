// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkRenderPass.h"
#include "VkDevice.h"
#include "VkImageView.h"

GENERATE_BODY(SVkRenderPass);

SVkRenderPass::SVkRenderPass(SVkDevice* Owner)
	: Super(Owner)
	, vkDevice(Owner->vkDevice)
{
}

void SVkRenderPass::AddColorAttachment(const RHIRenderPassAttachment& AttachDesc)
{
	vkAttachments.emplace_back() =
	{
		.format = AsVkFormat(AttachDesc.Format),
		.samples = AsVkSampleCountBits(AttachDesc.SampleDesc),
		.loadOp = AsVkAttachmentLoadOp(AttachDesc.LoadOp),
		.storeOp = AsVkAttachmentStoreOp(AttachDesc.StoreOp),
		.initialLayout = AsVkImageLayout(AttachDesc.BeforeState),
		.finalLayout = AsVkImageLayout(AttachDesc.AfterState)
	};

	vkColorAttachments.emplace_back() =
	{
		.attachment = VK_ATTACHMENT_UNUSED
	};

	bRecreateRenderPass = true;
}

void SVkRenderPass::Apply(std::span<std::pair<IRHIView*, int32> const> AttachViews, const Vector2N& Size)
{
	if (bRecreateRenderPass)
	{
		if (vkRenderPass != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(vkDevice, vkRenderPass, nullptr);
			vkRenderPass = VK_NULL_HANDLE;
		}

		VkSubpassDescription vkSubpass =
		{
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.colorAttachmentCount = (uint32_t)vkColorAttachments.size(),
			.pColorAttachments = vkColorAttachments.data(),
		};

		VkRenderPassCreateInfo vkRenderPassCreate =
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = (uint32_t)vkAttachments.size(),
			.pAttachments = vkAttachments.data(),
			.subpassCount = 1,
			.pSubpasses = &vkSubpass,
			.dependencyCount = 0,
			.pDependencies = nullptr
		};

		VR(vkCreateRenderPass(vkDevice, &vkRenderPassCreate, nullptr, &vkRenderPass));
		bRecreateRenderPass = false;
	}

	if (vkFramebuffer != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(vkDevice, vkFramebuffer, nullptr);
		vkFramebuffer = VK_NULL_HANDLE;
	}

	std::vector<VkImageView> vkImageViews;
	for (auto& [View, IndexOf] : AttachViews)
	{
		if (auto* sView = Cast<SVkImageView>(View))
		{
			vkImageViews.emplace_back(sView->GetVkImageView(IndexOf));
		}
	}

	VkFramebufferCreateInfo vkFramebufferCreate =
	{
		.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		.renderPass = vkRenderPass,
		.attachmentCount = (uint32_t)vkImageViews.size(),
		.pAttachments = vkImageViews.data(),
		.width = (uint32_t)Size.X,
		.height = (uint32_t)Size.Y,
		.layers = 1
	};

	VR(vkCreateFramebuffer(vkDevice, &vkFramebufferCreate, nullptr, &vkFramebuffer));
}

void SVkRenderPass::Dispose(bool bDisposing)
{
	if (vkFramebuffer != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(vkDevice, vkFramebuffer, nullptr);
		vkFramebuffer = VK_NULL_HANDLE;
	}

	if (vkRenderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(vkDevice, vkRenderPass, nullptr);
		vkRenderPass = VK_NULL_HANDLE;
	}

	Super::Dispose(bDisposing);
}