// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "VkCommandBuffer.h"
#include "VkDevice.h"
#include "VkCommandAllocator.h"
#include "VkImageView.h"
#include "VkImage.h"
#include "VkRenderPass.h"

GENERATE_BODY(SVkCommandBuffer);

SVkCommandBuffer::SVkCommandBuffer(SVkDevice* Owner, VkCommandPool vkCommandPool, VkCommandBuffer vkCommandBuffer)
	: Super(Owner)
	, vkCommandPool(std::move(vkCommandPool))
	, vkCommandBuffer(std::move(vkCommandBuffer))
{
	vkDevice = Owner->vkDevice;
}

void SVkCommandBuffer::BeginRecord()
{
	VkCommandBufferBeginInfo vkBeginInfo =
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
	};

	VR(vkBeginCommandBuffer(vkCommandBuffer, &vkBeginInfo));
}

void SVkCommandBuffer::EndRecord()
{
	VR(vkEndCommandBuffer(vkCommandBuffer));
}

void SVkCommandBuffer::ResourceBarrier(std::span<const RHIResourceBarrier> InBarriers)
{
	std::vector<VkImageMemoryBarrier> vkBarriers;

	for (auto& Barrier : InBarriers)
	{
		switch (Barrier.index())
		{
		case 0:
		{
			auto& Transit = std::get<RHIResourceTransitionBarrier>(Barrier);
			VkImage vkImage = Cast<SVkImage>(Transit.pResource)->vkImage;

			vkBarriers.emplace_back() =
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.oldLayout = AsVkImageLayout(Transit.StateBefore),
				.newLayout = AsVkImageLayout(Transit.StateAfter),
				.image = vkImage,
				.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.levelCount = 1,
					.layerCount = 1
				}
			};
			break;
		}
		default:
			checkf(false, L"NOT IMPLEMENTED.");
		}
	}

	vkCmdPipelineBarrier(vkCommandBuffer,
		VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT, VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
		VK_DEPENDENCY_BY_REGION_BIT,
		0, nullptr,
		0, nullptr,
		(uint32_t)vkBarriers.size(), vkBarriers.data());
}

void SVkCommandBuffer::ClearRenderTargetView(IRHIRenderTargetView* RTV, int32 IndexOf, const Color& InColor)
{
	auto* sRTV = Cast<SVkImageView>(RTV);
	VkImage vkImage = sRTV->GetVkImage(IndexOf);

	VkClearColorValue vkClear = {};
	memcpy(vkClear.float32, &InColor, sizeof(InColor));

	VkImageSubresourceRange vkRange
	{
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.levelCount = 1,
		.layerCount = 1
	};
	vkCmdClearColorImage(vkCommandBuffer, vkImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &vkClear, 1, &vkRange);
}

void SVkCommandBuffer::BeginRenderPass(IRHIRenderPass* RenderPass, const RectN& ScissorRect, std::span<Color const> InColor)
{
	auto* sRenderPass = Cast<SVkRenderPass>(RenderPass);

	std::vector<VkClearValue> vkClearColors(InColor.size());
	for (size_t i = 0; i < InColor.size(); ++i)
	{
		auto& vkClearColor = vkClearColors[i];
		memcpy(vkClearColor.color.float32, &InColor[i], sizeof(Color));
	}

	VkRenderPassBeginInfo vkBeginInfo =
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = sRenderPass->vkRenderPass,
		.framebuffer = sRenderPass->vkFramebuffer,
		.renderArea = VkRect2D{ ScissorRect.Left, ScissorRect.Top, (uint32_t)Rect::Width(ScissorRect), (uint32_t)Rect::Height(ScissorRect) },
		.clearValueCount = (uint32_t)vkClearColors.size(),
		.pClearValues = vkClearColors.data()
	};

	vkCmdBeginRenderPass(vkCommandBuffer, &vkBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SVkCommandBuffer::EndRenderPass()
{
	vkCmdEndRenderPass(vkCommandBuffer);
}