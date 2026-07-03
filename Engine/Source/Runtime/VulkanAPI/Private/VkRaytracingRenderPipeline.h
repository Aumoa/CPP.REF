// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderPipeline.h"
#include "VkCommon.h"
#include "VkRaytracingRenderPipeline.gen.h"

namespace Ayla
{
	class Shader;
	class VkGraphics;

	ACLASS()
	class VkRaytracingRenderPipeline : public RenderPipeline
	{
		GENERATED_BODY()

	private:
		VkGraphics* m_Graphics;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		uint32 m_RayGenerationGroupIndex = VK_SHADER_UNUSED_KHR;
		uint32 m_MissGroupIndex = VK_SHADER_UNUSED_KHR;
		uint32 m_HitGroupIndex = VK_SHADER_UNUSED_KHR;

	public:
		VkRaytracingRenderPipeline(VkGraphics* graphics, SharedPtr<Shader> shader);
		virtual ~VkRaytracingRenderPipeline() noexcept override;

		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) override;

		VkPipeline GetPipeline() const noexcept { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const noexcept { return m_PipelineLayout; }
		uint32 GetRayGenerationGroupIndex() const noexcept { return m_RayGenerationGroupIndex; }
		uint32 GetMissGroupIndex() const noexcept { return m_MissGroupIndex; }
		uint32 GetHitGroupIndex() const noexcept { return m_HitGroupIndex; }
	};
}
