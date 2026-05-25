// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderPipeline.h"
#include "VkCommon.h"
#include "VkGeometryRenderPipeline.gen.h"

namespace Ayla
{
	class Shader;
	class VkGraphics;

	ACLASS()
	class VkGeometryRenderPipeline : public RenderPipeline
	{
		GENERATED_BODY()

	private:
		VkGraphics* m_Graphics;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> m_DescriptorSets;

	public:
		VkGeometryRenderPipeline(VkGraphics* graphics, SharedPtr<Shader> shader);
		virtual ~VkGeometryRenderPipeline() noexcept override;

		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) override;

		VkPipeline GetPipeline() const noexcept { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const noexcept { return m_PipelineLayout; }
		VkRenderPass GetRenderPass() const noexcept { return m_RenderPass; }
	};
}
