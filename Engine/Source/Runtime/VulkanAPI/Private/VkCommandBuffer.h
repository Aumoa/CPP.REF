// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommandBuffer.h"
#include "VkCommon.h"
#include "VkCommandBuffer.gen.h"

namespace Ayla
{
	class VkGraphics;
	class VkRaytracingRenderPipeline;

	ACLASS()
	class VkCommandBuffer : public CommandBuffer
	{
		GENERATED_BODY()

	private:
		VkGraphics* m_Graphics{ nullptr };
		VkCommandPool m_CommandPool{ nullptr };
		std::vector<::VkCommandBuffer> m_CommandBuffers;
		std::vector<VkSemaphore> m_SignalSemaphores;
		std::vector<VkSemaphore> m_WaitSemaphores;
		std::vector<VkPipelineStageFlags> m_WaitSemaphoreStages;
		bool m_HasBegun{ false };
		std::vector<VkFence> m_Fences;
		VkRaytracingRenderPipeline* m_CurrentRaytracingRenderPipeline = nullptr;

		::VkBuffer m_VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_VertexMemory = VK_NULL_HANDLE;
		::VkBuffer m_IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_IndexMemory = VK_NULL_HANDLE;

	public:
		VkCommandBuffer(VkGraphics* graphics, bool fence);
		virtual ~VkCommandBuffer() noexcept override;

		virtual void Dispose() noexcept override;

		virtual void BeginCommands_Implementation() override;
		virtual void EndCommands_Implementation() override;
		virtual void BeginRenderPass(RenderTexture* renderTexture) override;
		virtual void EndRenderPass(RenderTexture* renderTexture) override;
		virtual void SetRenderPipeline(RenderPipeline* renderPipeline) override;
		virtual void Draw() override;
		virtual void DispatchRays(RenderTexture* renderTexture) override;
		virtual void WaitForCompletion(const TimeSpan& timeout) override;

		void AddSignalSemaphore(VkSemaphore semaphore);
		void AddWaitSemaphore(VkSemaphore semaphore, VkPipelineStageFlags stage);

		::VkCommandBuffer GetVkCommandBuffer() const noexcept;
		VkFence GetFence() const noexcept;

	private:
		void CreateTriangleBuffers();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	};
}
