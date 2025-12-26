// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "VkCommon.h"
#include "Numerics/VectorInterface/Vector.h"

namespace Ayla
{
	class VkGraphics;

	// Helper class for managing Vulkan acceleration structures
	class VkAccelerationStructure
	{
	private:
		VkGraphics* m_Graphics{ nullptr };
		VkAccelerationStructureKHR m_AccelerationStructure{ VK_NULL_HANDLE };
		::VkBuffer m_Buffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_Memory{ VK_NULL_HANDLE };
		VkDeviceAddress m_DeviceAddress{ 0 };

	public:
		VkAccelerationStructure(VkGraphics* graphics);
		~VkAccelerationStructure() noexcept;

		void CreateBottomLevel(const std::vector<Vector3F>& vertices, const std::vector<uint32_t>& indices);
		void CreateTopLevel(const VkAccelerationStructure& blas);

		VkAccelerationStructureKHR GetHandle() const noexcept { return m_AccelerationStructure; }
		VkDeviceAddress GetDeviceAddress() const noexcept { return m_DeviceAddress; }

	private:
		void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage);
	};

	// Helper class for managing raytracing pipeline and shader binding table
	class VkRaytracingPipeline
	{
	private:
		VkGraphics* m_Graphics{ nullptr };
		VkPipeline m_Pipeline{ VK_NULL_HANDLE };
		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkDescriptorSetLayout m_DescriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorPool m_DescriptorPool{ VK_NULL_HANDLE };
		VkDescriptorSet m_DescriptorSet{ VK_NULL_HANDLE };

		// Shader Binding Table
		::VkBuffer m_SbtBuffer{ VK_NULL_HANDLE };
		VkDeviceMemory m_SbtMemory{ VK_NULL_HANDLE };
		VkStridedDeviceAddressRegionKHR m_RaygenRegion{};
		VkStridedDeviceAddressRegionKHR m_MissRegion{};
		VkStridedDeviceAddressRegionKHR m_HitRegion{};
		VkStridedDeviceAddressRegionKHR m_CallableRegion{};

	public:
		VkRaytracingPipeline(VkGraphics* graphics);
		~VkRaytracingPipeline() noexcept;

		void CreatePipeline();
		void CreateShaderBindingTable();
		void UpdateDescriptorSet(VkAccelerationStructureKHR tlas, VkImageView outputImageView);

		VkPipeline GetPipeline() const noexcept { return m_Pipeline; }
		VkPipelineLayout GetPipelineLayout() const noexcept { return m_PipelineLayout; }
		VkDescriptorSet GetDescriptorSet() const noexcept { return m_DescriptorSet; }

		const VkStridedDeviceAddressRegionKHR& GetRaygenRegion() const noexcept { return m_RaygenRegion; }
		const VkStridedDeviceAddressRegionKHR& GetMissRegion() const noexcept { return m_MissRegion; }
		const VkStridedDeviceAddressRegionKHR& GetHitRegion() const noexcept { return m_HitRegion; }
		const VkStridedDeviceAddressRegionKHR& GetCallableRegion() const noexcept { return m_CallableRegion; }

	private:
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);
	};
}
