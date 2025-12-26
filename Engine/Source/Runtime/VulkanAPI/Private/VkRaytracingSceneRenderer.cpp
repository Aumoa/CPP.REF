// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "VkRaytracingSceneRenderer.h"
#include "VkGraphics.h"
#include "VkRaytracingHelper.h"
#include "VkCommandBuffer.h"
#include "CommandBuffer.h"

namespace Ayla
{
	VkRaytracingSceneRenderer::VkRaytracingSceneRenderer(VkGraphics* graphics)
		: m_Graphics(graphics)
	{
	}

	VkRaytracingSceneRenderer::~VkRaytracingSceneRenderer() noexcept
	{
	}

	void VkRaytracingSceneRenderer::Initialize()
	{
		if (m_Initialized)
		{
			return;
		}

		CreateTriangle();

		m_Pipeline = std::make_unique<VkRaytracingPipeline>(m_Graphics);
		m_Pipeline->CreatePipeline();
		m_Pipeline->CreateShaderBindingTable();

		m_Initialized = true;
	}

	void VkRaytracingSceneRenderer::CreateTriangle()
	{
		// Define a simple triangle
		std::vector<Vector3F> vertices = {
			Vector3F(0.0f, -0.5f, 0.0f),
			Vector3F(0.5f, 0.5f, 0.0f),
			Vector3F(-0.5f, 0.5f, 0.0f)
		};

		std::vector<uint32_t> indices = { 0, 1, 2 };

		// Create BLAS for triangle
		m_Blas = std::make_unique<VkAccelerationStructure>(m_Graphics);
		m_Blas->CreateBottomLevel(vertices, indices);

		// Create TLAS
		m_Tlas = std::make_unique<VkAccelerationStructure>(m_Graphics);
		m_Tlas->CreateTopLevel(*m_Blas);
	}

	void VkRaytracingSceneRenderer::Render(CommandBuffer* cmd, VkImage outputImage, VkImageView outputImageView, Vector2N size)
	{
		if (!m_Initialized)
		{
			Initialize();
		}

		auto* vkCmd = static_cast<VkCommandBuffer*>(cmd);
		auto commandBuffer = vkCmd->GetVkCommandBuffer();

		// Transition image to general layout for raytracing write
		VkImageMemoryBarrier toGeneralBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.newLayout = VK_IMAGE_LAYOUT_GENERAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = outputImage,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
			0,
			0, nullptr,
			0, nullptr,
			1, &toGeneralBarrier
		);

		// Update descriptor set with current TLAS and output image
		m_Pipeline->UpdateDescriptorSet(m_Tlas->GetHandle(), outputImageView);

		// Bind raytracing pipeline
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR, m_Pipeline->GetPipeline());

		// Bind descriptor sets
		auto descriptorSet = m_Pipeline->GetDescriptorSet();
		vkCmdBindDescriptorSets(
			commandBuffer,
			VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
			m_Pipeline->GetPipelineLayout(),
			0, 1, &descriptorSet,
			0, nullptr
		);

		// Trace rays
		auto vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)
			vkGetDeviceProcAddr(m_Graphics->GetDevice(), "vkCmdTraceRaysKHR");

		vkCmdTraceRaysKHR(
			commandBuffer,
			&m_Pipeline->GetRaygenRegion(),
			&m_Pipeline->GetMissRegion(),
			&m_Pipeline->GetHitRegion(),
			&m_Pipeline->GetCallableRegion(),
			size.X,
			size.Y,
			1
		);

		// Transition image back to present layout
		VkImageMemoryBarrier toPresentBarrier
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT,
			.dstAccessMask = 0,
			.oldLayout = VK_IMAGE_LAYOUT_GENERAL,
			.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = outputImage,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &toPresentBarrier
		);
	}
}
