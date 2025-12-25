// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "Rendering/RenderTexture.h"
#include "VkSwapchainRenderTexture.gen.h"

namespace Ayla
{
	class VkSwapchainExt;
	class VkCommandBuffer;
	class VkGraphics;

	ACLASS()
	class VkSwapchainRenderTexture : public RenderTexture
	{
		GENERATED_BODY()

	private:
		const VkSwapchainExt* m_Swapchain;
		VkGraphics* m_Graphics;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;
		std::vector<VkSemaphore> m_PresentCompletedSemaphores;
		std::vector<VkSemaphore> m_RenderCompletedSemaphores;

		uint32 m_CurrentImageIndex = 0xFFFFFFFF;
		uint8 m_SwapchainImageFirstRender = 0;

	public:
		VkSwapchainRenderTexture(VkSwapchainExt* swapchain, VkGraphics* graphics);

		virtual Vector2N GetSize() const override;

		virtual void Acquire(CommandBuffer* cmd) override;

		void Dispose();
		void Invalidate();
		void Present(VkQueue queue, VkCommandBuffer* vkCmd);

		VkImage GetCurrentImage() const { return m_SwapchainImages[m_CurrentImageIndex]; }
		VkImageView GetCurrentImageView() const { return m_SwapchainImageViews[m_CurrentImageIndex]; }

	private:
		void ReallocateSwapchainImages();
	};
}