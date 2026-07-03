// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "Rendering/PresentableRenderTarget.h"
#include "Rendering/RenderTexture.h"
#include "VkSwapchainRenderTexture.gen.h"

namespace Ayla
{
	class VkSwapchainExt;
	class VkCommandBuffer;
	class VkGraphics;

	ACLASS()
	class VkSwapchainRenderTexture : public RenderTexture, public PresentableRenderTarget
	{
		GENERATED_BODY()

	private:
		const VkSwapchainExt* m_Swapchain;
		VkGraphics* m_Graphics;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;
		std::vector<VkSemaphore> m_PresentCompletedSemaphores;
		std::vector<VkSemaphore> m_RenderCompletedSemaphores;

		VkImage m_DepthImage = VK_NULL_HANDLE;
		VkDeviceMemory m_DepthMemory = VK_NULL_HANDLE;
		VkImageView m_DepthImageView = VK_NULL_HANDLE;

		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		std::vector<VkFramebuffer> m_Framebuffers;

		uint32 m_CurrentImageIndex = 0xFFFFFFFF;
		uint8 m_SwapchainImageFirstRender = 0;

		static constexpr VkFormat kColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
		static constexpr VkFormat kDepthFormat = VK_FORMAT_D32_SFLOAT;

	public:
		VkSwapchainRenderTexture(VkSwapchainExt* swapchain, VkGraphics* graphics);

		virtual Vector2N GetSize() const override;

		virtual PresentableFrame AcquireFrame(CommandBuffer* commandBuffer) override;
		virtual void Present(CommandBuffer* commandBuffer) override;
		virtual void Invalidate() override;

		void Dispose();

		VkRenderPass GetRenderPass() const noexcept { return m_RenderPass; }
		VkFramebuffer GetCurrentFramebuffer() const noexcept;
		VkImage GetCurrentImage() const noexcept;
		VkImageView GetCurrentImageView() const noexcept;

		static VkFormat GetColorFormat() noexcept { return kColorFormat; }
		static VkFormat GetDepthFormat() noexcept { return kDepthFormat; }

	private:
		void ReallocateSwapchainImages();
		void CreateRenderPass();
		void CreateDepthResources();
		void CreateFramebuffers();
		void DestroyFramebufferResources();
	};
}
