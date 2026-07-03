// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/RenderPipeline.h"
#include "VkCommon.h"
#include "VkRaytracingRenderPipeline.gen.h"

namespace Ayla
{
	class Shader;
	class VkCommandBuffer;
	class VkGraphics;
	class VkSwapchainRenderTexture;

	ACLASS()
	class VkRaytracingRenderPipeline : public RenderPipeline
	{
		GENERATED_BODY()

	private:
		VkGraphics* m_Graphics;
		VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		VkPipeline m_Pipeline = VK_NULL_HANDLE;
		VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> m_DescriptorSets;
		::VkBuffer m_ShaderBindingTableBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_ShaderBindingTableMemory = VK_NULL_HANDLE;
		VkDeviceAddress m_ShaderBindingTableAddress = 0;
		VkStridedDeviceAddressRegionKHR m_RayGenerationShaderBindingTable = {};
		VkStridedDeviceAddressRegionKHR m_MissShaderBindingTable = {};
		VkStridedDeviceAddressRegionKHR m_HitShaderBindingTable = {};
		VkStridedDeviceAddressRegionKHR m_CallableShaderBindingTable = {};
		uint32 m_RayGenerationGroupIndex = VK_SHADER_UNUSED_KHR;
		uint32 m_MissGroupIndex = VK_SHADER_UNUSED_KHR;
		uint32 m_HitGroupIndex = VK_SHADER_UNUSED_KHR;

	public:
		VkRaytracingRenderPipeline(VkGraphics* graphics, SharedPtr<Shader> shader);
		virtual ~VkRaytracingRenderPipeline() noexcept override;

		virtual void SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset) override;

		VkPipeline GetPipeline() const noexcept { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const noexcept { return m_PipelineLayout; }
		const VkStridedDeviceAddressRegionKHR& GetRayGenerationShaderBindingTable() const noexcept { return m_RayGenerationShaderBindingTable; }
		const VkStridedDeviceAddressRegionKHR& GetMissShaderBindingTable() const noexcept { return m_MissShaderBindingTable; }
		const VkStridedDeviceAddressRegionKHR& GetHitShaderBindingTable() const noexcept { return m_HitShaderBindingTable; }
		const VkStridedDeviceAddressRegionKHR& GetCallableShaderBindingTable() const noexcept { return m_CallableShaderBindingTable; }
		uint32 GetRayGenerationGroupIndex() const noexcept { return m_RayGenerationGroupIndex; }
		uint32 GetMissGroupIndex() const noexcept { return m_MissGroupIndex; }
		uint32 GetHitGroupIndex() const noexcept { return m_HitGroupIndex; }
		void BindOutputTexture(VkCommandBuffer* cmd, VkSwapchainRenderTexture* renderTexture);

	private:
		void CreateDescriptorSets();
		void CreateShaderBindingTable(uint32 shaderGroupCount);
	};
}
