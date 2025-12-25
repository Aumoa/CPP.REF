// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RaytracingSceneRenderer.h"
#include "VkCommon.h"
#include <memory>

namespace Ayla
{
	class VkGraphics;
	class VkAccelerationStructure;
	class VkRaytracingPipeline;
	class CommandBuffer;

	class VkRaytracingSceneRenderer
	{
	private:
		VkGraphics* m_Graphics{ nullptr };
		std::unique_ptr<VkAccelerationStructure> m_Blas;
		std::unique_ptr<VkAccelerationStructure> m_Tlas;
		std::unique_ptr<VkRaytracingPipeline> m_Pipeline;
		bool m_Initialized{ false };

	public:
		VkRaytracingSceneRenderer(VkGraphics* graphics);
		~VkRaytracingSceneRenderer() noexcept;

		void Initialize();
		void Render(CommandBuffer* cmd, VkImage outputImage, VkImageView outputImageView, Vector2N size);

	private:
		void CreateTriangle();
	};
}
