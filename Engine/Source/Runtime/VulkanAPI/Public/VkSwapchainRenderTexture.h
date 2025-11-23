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

	ACLASS()
	class VkSwapchainRenderTexture : public RenderTexture
	{
		GENERATED_BODY()

	private:
		const VkSwapchainExt* m_Swapchain;
		std::vector<VkImage> m_SwapchainImages;
		VkSemaphore m_ImageReadySemaphore = VK_NULL_HANDLE;

		uint32 m_CurrentImageIndex = 0xFFFFFFFF;
		uint8 m_SwapchainImageFirstRender = 0;

	public:
		VkSwapchainRenderTexture(VkSwapchainExt* swapchain);

		virtual Vector2N GetSize() const override;

		virtual void Acquire(CommandBuffer* cmd) override;

		void Invalidate();
		void Present(VkQueue queue, VkCommandBuffer* vkCmd);

	private:
		void ReallocateSwapchainImages();
	};
}