// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkCommandBuffer.h"
#include "VkGraphics.h"
#include "VkCommandQueue.h"
#include "VkSwapchainRenderTexture.h"
#include "VkGeometryRenderPipeline.h"
#include "VkRaytracingRenderPipeline.h"
#include "Misc/PositionColorVertex.h"

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

		CreateTriangleBuffers();
	}

	VkCommandBuffer::~VkCommandBuffer() noexcept
	{
		Dispose();
	}

	void VkCommandBuffer::Dispose() noexcept
	{
		auto device = m_Graphics->GetDevice();

		if (m_VertexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, m_VertexBuffer, nullptr);
			m_VertexBuffer = VK_NULL_HANDLE;
		}
		if (m_VertexMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_VertexMemory, nullptr);
			m_VertexMemory = VK_NULL_HANDLE;
		}
		if (m_IndexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device, m_IndexBuffer, nullptr);
			m_IndexBuffer = VK_NULL_HANDLE;
		}
		if (m_IndexMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device, m_IndexMemory, nullptr);
			m_IndexMemory = VK_NULL_HANDLE;
		}

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

		if (m_Fences.size() > 0)
		{
			VKR(vkResetFences(m_Graphics->GetDevice(), 1, &m_Fences[frameIndex]));
		}

		m_SignalSemaphores.clear();
		m_WaitSemaphores.clear();

		VkCommandBufferBeginInfo beginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO
		};

		auto commandBuffer = m_CommandBuffers[frameIndex];

		vkResetCommandBuffer(commandBuffer, 0);
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

		static thread_local std::vector<VkPipelineStageFlags> sStages;
		sStages.resize(m_WaitSemaphores.size(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		submitInfo.pWaitDstStageMask = sStages.data();
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

	void VkCommandBuffer::Draw()
	{
		VkDeviceSize offset = 0;
		vkCmdBindVertexBuffers(GetVkCommandBuffer(), 0, 1, &m_VertexBuffer, &offset);
		vkCmdBindIndexBuffer(GetVkCommandBuffer(), m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(GetVkCommandBuffer(), 3, 1, 0, 0, 0);
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

		VkImageMemoryBarrier prepareStorageWriteBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.newLayout = VK_IMAGE_LAYOUT_GENERAL,
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
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
			0,
			0, nullptr,
			0, nullptr,
			1, &prepareStorageWriteBarrier
		);

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

		VkImageMemoryBarrier preparePresentBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.dstAccessMask = 0,
			.oldLayout = VK_IMAGE_LAYOUT_GENERAL,
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
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
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

	void VkCommandBuffer::AddWaitSemaphore(VkSemaphore semaphore)
	{
		m_WaitSemaphores.emplace_back(semaphore);
	}

	::VkCommandBuffer VkCommandBuffer::GetVkCommandBuffer() const noexcept
	{
		return m_CommandBuffers[m_Graphics->GetFrameIndex()];
	}

	VkFence VkCommandBuffer::GetFence() const noexcept
	{
		return m_Fences[m_Graphics->GetFrameIndex()];
	}

	void VkCommandBuffer::CreateTriangleBuffers()
	{
		auto device = m_Graphics->GetDevice();

		// Create vertex buffer (host visible, like D3D12's upload heap)
		PositionColorVertex vertices[3] =
		{
			{ Vector3F(0, 1.0f, 0), NamedColors::Red },
			{ Vector3F(1.0f, -1.0f, 0), NamedColors::Green },
			{ Vector3F(-1.0f, -1.0f, 0), NamedColors::Blue },
		};

		VkBufferCreateInfo vertexBufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(vertices),
			.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		VKR(vkCreateBuffer(device, &vertexBufferInfo, nullptr, &m_VertexBuffer));

		VkMemoryRequirements vertexMemReq;
		vkGetBufferMemoryRequirements(device, m_VertexBuffer, &vertexMemReq);

		VkMemoryAllocateInfo vertexAllocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = vertexMemReq.size,
			.memoryTypeIndex = FindMemoryType(vertexMemReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		};

		VKR(vkAllocateMemory(device, &vertexAllocInfo, nullptr, &m_VertexMemory));
		VKR(vkBindBufferMemory(device, m_VertexBuffer, m_VertexMemory, 0));

		void* vertexData;
		VKR(vkMapMemory(device, m_VertexMemory, 0, sizeof(vertices), 0, &vertexData));
		std::memcpy(vertexData, vertices, sizeof(vertices));
		vkUnmapMemory(device, m_VertexMemory);

		// Create index buffer (host visible, like D3D12's upload heap)
		uint32 indices[3] = { 0, 1, 2 };

		VkBufferCreateInfo indexBufferInfo
		{
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = sizeof(indices),
			.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		};

		VKR(vkCreateBuffer(device, &indexBufferInfo, nullptr, &m_IndexBuffer));

		VkMemoryRequirements indexMemReq;
		vkGetBufferMemoryRequirements(device, m_IndexBuffer, &indexMemReq);

		VkMemoryAllocateInfo indexAllocInfo
		{
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = indexMemReq.size,
			.memoryTypeIndex = FindMemoryType(indexMemReq.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
		};

		VKR(vkAllocateMemory(device, &indexAllocInfo, nullptr, &m_IndexMemory));
		VKR(vkBindBufferMemory(device, m_IndexBuffer, m_IndexMemory, 0));

		void* indexData;
		VKR(vkMapMemory(device, m_IndexMemory, 0, sizeof(indices), 0, &indexData));
		std::memcpy(indexData, indices, sizeof(indices));
		vkUnmapMemory(device, m_IndexMemory);
	}

	uint32_t VkCommandBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
	{
		return m_Graphics->FindMemoryType(typeFilter, properties);
	}

	SharedPtr<CommandBuffer> VkGraphics::CreateCommandBuffer()
	{
		return New<VkCommandBuffer>(this, true);
	}
}
