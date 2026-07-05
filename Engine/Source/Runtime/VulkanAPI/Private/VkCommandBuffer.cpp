// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkCommandBuffer.h"
#include "VkGraphics.h"
#include "VkCommandQueue.h"
#include "VkSwapchainRenderTexture.h"
#include "VkGeometryRenderPipeline.h"
#include "VkRaytracingRenderPipeline.h"
#include "VkBuffer.h"

namespace Ayla
{
	VkCommandBuffer::VkCommandBuffer(VkGraphics* graphics, bool fence)
		: m_Graphics{ graphics }
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
			.queueFamilyIndex = (uint32_t)graphics->GetGraphicsQueue()->GetQueueFamilyIndex()
		};

		VKR(vkCreateCommandPool(graphics->GetDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = m_CommandPool,
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = (uint32_t)VkGraphics::kMaxFramesInFlight
		};

		m_CommandBuffers.resize(VkGraphics::kMaxFramesInFlight);
		VKR(vkAllocateCommandBuffers(graphics->GetDevice(), &commandBufferAllocInfo, m_CommandBuffers.data()));

		if (fence)
		{
			VkFenceCreateInfo fenceCreateInfo
			{
				.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT
			};

			m_Fences.resize(VkGraphics::kMaxFramesInFlight);
			for (size_t i = 0; i < VkGraphics::kMaxFramesInFlight; ++i)
			{
				VKR(vkCreateFence(graphics->GetDevice(), &fenceCreateInfo, nullptr, &m_Fences[i]));
			}
		}
	}

	VkCommandBuffer::~VkCommandBuffer() noexcept
	{
		Dispose();
	}

	void VkCommandBuffer::Dispose() noexcept
	{
		auto device = m_Graphics->GetDevice();

		for (auto& fence : m_Fences)
		{
			vkDestroyFence(device, fence, nullptr);
		}
		m_Fences.clear();

		if (m_CommandBuffers.size() > 0)
		{
			vkFreeCommandBuffers(device, m_CommandPool, (uint32_t)m_CommandBuffers.size(), m_CommandBuffers.data());
			m_CommandBuffers.clear();
		}

		if (m_CommandPool)
		{
			vkDestroyCommandPool(device, m_CommandPool, nullptr);
			m_CommandPool = nullptr;
		}
	}

	void VkCommandBuffer::BeginCommands_Implementation()
	{
		auto frameIndex = m_Graphics->GetFrameIndex();
		auto device = m_Graphics->GetDevice();

		if (m_Fences.size() > 0)
		{
			VKR(vkWaitForFences(device, 1, &m_Fences[frameIndex], VK_TRUE, UINT64_MAX));
			VKR(vkResetFences(device, 1, &m_Fences[frameIndex]));
		}

		m_SignalSemaphores.clear();
		m_WaitSemaphores.clear();
		m_WaitSemaphoreStages.clear();

		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

		auto commandBuffer = m_CommandBuffers[frameIndex];

		VKR(vkResetCommandBuffer(commandBuffer, 0));
		VKR(vkBeginCommandBuffer(commandBuffer, &beginInfo));
	}

	void VkCommandBuffer::EndCommands_Implementation()
	{
		auto frameIndex = m_Graphics->GetFrameIndex();
		auto commandBuffer = m_CommandBuffers[frameIndex];
		VKR(vkEndCommandBuffer(commandBuffer));

		VkSubmitInfo submitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &commandBuffer
		};

		submitInfo.signalSemaphoreCount = (uint32_t)m_SignalSemaphores.size();
		submitInfo.pSignalSemaphores = m_SignalSemaphores.data();

		submitInfo.pWaitDstStageMask = m_WaitSemaphoreStages.data();
		submitInfo.waitSemaphoreCount = (uint32_t)m_WaitSemaphores.size();
		submitInfo.pWaitSemaphores = m_WaitSemaphores.data();
		auto fence = m_Fences.size() > 0 ? m_Fences[frameIndex] : VK_NULL_HANDLE;
		VKR(vkQueueSubmit(m_Graphics->GetGraphicsQueue()->GetVkQueue(), 1, &submitInfo, fence));
	}

	void VkCommandBuffer::BeginRenderPass(RenderTexture* renderTexture)
	{
		if (auto* rt = dynamic_cast<VkSwapchainRenderTexture*>(renderTexture))
		{
			auto size = rt->GetSize();
			auto framebuffer = rt->GetCurrentFramebuffer();
			auto renderPass = rt->GetRenderPass();

			VkClearValue clearValues[2] = {};
			clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 1.0f } };
			clearValues[1].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo renderPassBeginInfo
			{
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				.renderPass = renderPass,
				.framebuffer = framebuffer,
				.renderArea =
				{
					.offset = { 0, 0 },
					.extent = { (uint32_t)size.X, (uint32_t)size.Y },
				},
				.clearValueCount = 2,
				.pClearValues = clearValues,
			};

			vkCmdBeginRenderPass(GetVkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport
			{
				.x = 0.0f,
				.y = 0.0f,
				.width = static_cast<float>(size.X),
				.height = static_cast<float>(size.Y),
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};
			vkCmdSetViewport(GetVkCommandBuffer(), 0, 1, &viewport);

			VkRect2D scissor
			{
				.offset = { 0, 0 },
				.extent = { (uint32_t)size.X, (uint32_t)size.Y },
			};
			vkCmdSetScissor(GetVkCommandBuffer(), 0, 1, &scissor);
		}
		else
		{
			throw InvalidOperationException(TEXT("Unsupported render texture type."));
		}
	}

	void VkCommandBuffer::EndRenderPass(RenderTexture* renderTexture)
	{
		vkCmdEndRenderPass(GetVkCommandBuffer());
	}

	void VkCommandBuffer::SetRenderPipeline(RenderPipeline* renderPipeline)
	{
		if (auto* pso = dynamic_cast<VkRaytracingRenderPipeline*>(renderPipeline))
		{
			vkCmdBindPipeline(GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, pso->GetPipeline());
			m_CurrentRaytracingRenderPipeline = pso;
		}
		else if (auto* ps = dynamic_cast<VkGeometryRenderPipeline*>(renderPipeline))
		{
			vkCmdBindPipeline(GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, ps->GetPipeline());
			m_CurrentRaytracingRenderPipeline = nullptr;
		}
		else
		{
			throw InvalidOperationException(TEXT("Unsupported render pipeline type."));
		}
	}

	void VkCommandBuffer::Draw(Buffer* vertexBuffer, Buffer* indexBuffer)
	{
		auto* vkVertexBuffer = dynamic_cast<VkBuffer*>(vertexBuffer);
		if (vkVertexBuffer == nullptr || vkVertexBuffer->GetUsage() != BufferUsage::VertexBuffer)
		{
			throw InvalidOperationException(TEXT("Vulkan draw requires a Vulkan vertex buffer."));
		}

		auto* vkIndexBuffer = dynamic_cast<VkBuffer*>(indexBuffer);
		if (vkIndexBuffer == nullptr || vkIndexBuffer->GetUsage() != BufferUsage::IndexBuffer)
		{
			throw InvalidOperationException(TEXT("Vulkan draw requires a Vulkan index buffer."));
		}

		const size_t indexStride = vkIndexBuffer->GetStride();
		if (indexStride != sizeof(uint16) && indexStride != sizeof(uint32))
		{
			throw InvalidOperationException(TEXT("Vulkan draw requires a 16-bit or 32-bit index buffer."));
		}

		VkDeviceSize offset = 0;
		::VkBuffer nativeVertexBuffer = vkVertexBuffer->GetVkBuffer();
		const VkIndexType indexType = (indexStride == sizeof(uint16)) ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
		vkCmdBindVertexBuffers(GetVkCommandBuffer(), 0, 1, &nativeVertexBuffer, &offset);
		vkCmdBindIndexBuffer(GetVkCommandBuffer(), vkIndexBuffer->GetVkBuffer(), 0, indexType);
		vkCmdDrawIndexed(GetVkCommandBuffer(), static_cast<uint32_t>(vkIndexBuffer->GetCount()), 1, 0, 0, 0);
	}

	void VkCommandBuffer::DispatchRays(RenderTexture* renderTexture)
	{
		if (m_CurrentRaytracingRenderPipeline == nullptr)
		{
			throw InvalidOperationException(TEXT("A Vulkan raytracing pipeline must be bound before dispatching rays."));
		}

		auto* rt = dynamic_cast<VkSwapchainRenderTexture*>(renderTexture);
		if (rt == nullptr)
		{
			throw InvalidOperationException(TEXT("Vulkan ray dispatch requires a Vulkan swapchain render texture."));
		}

		if (rt->GetRaytracingOutputImage() == VK_NULL_HANDLE || rt->GetRaytracingOutputImageView() == VK_NULL_HANDLE)
		{
			throw InvalidOperationException(TEXT("Vulkan ray dispatch requires an allocated raytracing output image."));
		}

		auto raytracingOutputImageLayout = rt->GetRaytracingOutputImageLayout();
		VkPipelineStageFlags raytracingOutputSourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		VkAccessFlags raytracingOutputSourceAccess = 0;
		if (raytracingOutputImageLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
		{
			raytracingOutputSourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			raytracingOutputSourceAccess = VK_ACCESS_TRANSFER_READ_BIT;
		}

		VkImageMemoryBarrier prepareStorageWriteBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = raytracingOutputSourceAccess,
			.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.oldLayout = raytracingOutputImageLayout,
			.newLayout = VK_IMAGE_LAYOUT_GENERAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = rt->GetRaytracingOutputImage(),
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		vkCmdPipelineBarrier(
			GetVkCommandBuffer(),
			raytracingOutputSourceStage,
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
			0,
			0, nullptr,
			0, nullptr,
			1, &prepareStorageWriteBarrier
		);
		rt->SetRaytracingOutputImageLayout(VK_IMAGE_LAYOUT_GENERAL);

		m_CurrentRaytracingRenderPipeline->BindOutputTexture(this, rt);

		auto size = renderTexture->GetSize();
		m_Graphics->GetCmdTraceRaysKHRFunction()(
			GetVkCommandBuffer(),
			&m_CurrentRaytracingRenderPipeline->GetRayGenerationShaderBindingTable(),
			&m_CurrentRaytracingRenderPipeline->GetMissShaderBindingTable(),
			&m_CurrentRaytracingRenderPipeline->GetHitShaderBindingTable(),
			&m_CurrentRaytracingRenderPipeline->GetCallableShaderBindingTable(),
			static_cast<uint32_t>(size.X),
			static_cast<uint32_t>(size.Y),
			1
		);

		std::array<VkImageMemoryBarrier, 2> prepareBlitBarriers =
		{
			VkImageMemoryBarrier
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
				.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_GENERAL,
				.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = rt->GetRaytracingOutputImage(),
				.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},
			VkImageMemoryBarrier
			{
				.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.srcAccessMask = 0,
				.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
				.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image = rt->GetCurrentImage(),
				.subresourceRange =
				{
					.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel = 0,
					.levelCount = 1,
					.baseArrayLayer = 0,
					.layerCount = 1,
				},
			},
		};

		vkCmdPipelineBarrier(
			GetVkCommandBuffer(),
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			static_cast<uint32_t>(prepareBlitBarriers.size()), prepareBlitBarriers.data()
		);
		rt->SetRaytracingOutputImageLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

		VkImageBlit blitRegion
		{
			.srcSubresource =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.srcOffsets =
			{
				VkOffset3D { 0, 0, 0 },
				VkOffset3D { static_cast<int32_t>(size.X), static_cast<int32_t>(size.Y), 1 },
			},
			.dstSubresource =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
			.dstOffsets =
			{
				VkOffset3D { 0, 0, 0 },
				VkOffset3D { static_cast<int32_t>(size.X), static_cast<int32_t>(size.Y), 1 },
			},
		};

		vkCmdBlitImage(
			GetVkCommandBuffer(),
			rt->GetRaytracingOutputImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			rt->GetCurrentImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blitRegion,
			VK_FILTER_NEAREST
		);

		VkImageMemoryBarrier preparePresentBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
			.dstAccessMask = 0,
			.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = rt->GetCurrentImage(),
			.subresourceRange =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		vkCmdPipelineBarrier(
			GetVkCommandBuffer(),
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &preparePresentBarrier
		);
	}

	void VkCommandBuffer::WaitForCompletion(const TimeSpan& timeout)
	{
		if (m_Fences.empty())
		{
			throw InvalidOperationException(TEXT("This command buffer was not created with a fence."));
		}

		auto frameIndex = m_Graphics->GetFrameIndex();
		VKR(vkWaitForFences(m_Graphics->GetDevice(), 1, &m_Fences[frameIndex], VK_TRUE, (uint64_t)timeout.GetTotalNanoseconds()));
	}

	void VkCommandBuffer::AddSignalSemaphore(VkSemaphore semaphore)
	{
		m_SignalSemaphores.emplace_back(semaphore);
	}

	void VkCommandBuffer::AddWaitSemaphore(VkSemaphore semaphore, VkPipelineStageFlags stage)
	{
		m_WaitSemaphores.emplace_back(semaphore);
		m_WaitSemaphoreStages.emplace_back(stage);
	}

	::VkCommandBuffer VkCommandBuffer::GetVkCommandBuffer() const noexcept
	{
		return m_CommandBuffers[m_Graphics->GetFrameIndex()];
	}

	VkFence VkCommandBuffer::GetFence() const noexcept
	{
		return m_Fences[m_Graphics->GetFrameIndex()];
	}

	SharedPtr<CommandBuffer> VkGraphics::CreateCommandBuffer()
	{
		return New<VkCommandBuffer>(this, true);
	}
}
