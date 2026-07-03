// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "MoveOnlyFunction.h"
#include "Graphics.h"
#include "VkCommon.h"
#include "VkGraphics.gen.h"

namespace Ayla
{
    class GenericApplication;
    class VkCommandQueue;

    ACLASS()
    class VULKANAPI_API VkGraphics : public Graphics
    {
        GENERATED_BODY()

    public:
        template<class TBody>
        using function_t = MoveOnlyFunction<TBody>;

    private:
        VkInstanceRef m_Instance;
        VkPhysicalDevice m_PhysicalDevice{ nullptr };
        VkDeviceRef m_Device;
        std::array<std::unique_ptr<VkCommandQueue>, 3> m_Queues;

        VkPhysicalDeviceRayTracingPipelinePropertiesKHR m_RaytracingPipelineProperties = {};
        VkPhysicalDeviceAccelerationStructurePropertiesKHR m_AccelerationStructureProperties = {};

        PFN_vkCreateRayTracingPipelinesKHR m_vkCreateRayTracingPipelinesKHR = nullptr;
        PFN_vkGetRayTracingShaderGroupHandlesKHR m_vkGetRayTracingShaderGroupHandlesKHR = nullptr;
        PFN_vkCmdTraceRaysKHR m_vkCmdTraceRaysKHR = nullptr;
        PFN_vkGetBufferDeviceAddress m_vkGetBufferDeviceAddress = nullptr;
        PFN_vkCreateAccelerationStructureKHR m_vkCreateAccelerationStructureKHR = nullptr;
        PFN_vkDestroyAccelerationStructureKHR m_vkDestroyAccelerationStructureKHR = nullptr;
        PFN_vkGetAccelerationStructureBuildSizesKHR m_vkGetAccelerationStructureBuildSizesKHR = nullptr;
        PFN_vkBuildAccelerationStructuresKHR m_vkBuildAccelerationStructuresKHR = nullptr;
        PFN_vkCmdBuildAccelerationStructuresKHR m_vkCmdBuildAccelerationStructuresKHR = nullptr;
        PFN_vkGetAccelerationStructureDeviceAddressKHR m_vkGetAccelerationStructureDeviceAddressKHR = nullptr;

        std::atomic<std::size_t> m_FrameCount = 0;

        std::mutex m_FenceCompletionMutex;
		std::map<VkFence, function_t<void()>> m_FenceCompletionCallbacks;

    public:
        ACONSTRUCTOR()
        VkGraphics();
        virtual ~VkGraphics() noexcept override;

        virtual void Dispose() noexcept override;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept override { return RenderFeatures::Vulkan; }

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) override;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer() override;  // VkCommandBuffer.cpp
        virtual SharedPtr<RenderPipeline> CreateGeometryRenderPipeline(SharedPtr<Shader> shader) override;
        virtual SharedPtr<RenderPipeline> CreateRaytracingRenderPipeline(SharedPtr<Shader> shader) override;
        virtual SharedPtr<Shader> CreateShader(ShaderCreationInfo shaderCreationInfo) override;
        virtual SharedPtr<Buffer> CreateUploadBuffer(size_t sizeInBytes) override; // VkBuffer.cpp
        virtual SharedPtr<Buffer> CreateBuffer(std::span<const byte> data, size_t stride, BufferUsage usage) override; // VkBuffer.cpp

        virtual void BeginRenderFrame() override;
        virtual void EndRenderFrame() override;
        virtual void WaitForCompletion() override;

        VkInstance GetInstance() const noexcept { return m_Instance; }
        VkDevice GetDevice() const noexcept { return m_Device; }
        VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_PhysicalDevice; }
        VkCommandQueue* GetGraphicsQueue() const noexcept { return m_Queues[0].get(); }
        const VkPhysicalDeviceRayTracingPipelinePropertiesKHR& GetRaytracingPipelineProperties() const noexcept { return m_RaytracingPipelineProperties; }
        const VkPhysicalDeviceAccelerationStructurePropertiesKHR& GetAccelerationStructureProperties() const noexcept { return m_AccelerationStructureProperties; }

        PFN_vkCreateRayTracingPipelinesKHR GetCreateRayTracingPipelinesKHRFunction() const noexcept { return m_vkCreateRayTracingPipelinesKHR; }
        PFN_vkGetRayTracingShaderGroupHandlesKHR GetRayTracingShaderGroupHandlesKHRFunction() const noexcept { return m_vkGetRayTracingShaderGroupHandlesKHR; }
        PFN_vkCmdTraceRaysKHR GetCmdTraceRaysKHRFunction() const noexcept { return m_vkCmdTraceRaysKHR; }
        PFN_vkGetBufferDeviceAddress GetBufferDeviceAddressFunction() const noexcept { return m_vkGetBufferDeviceAddress; }
        PFN_vkCreateAccelerationStructureKHR GetCreateAccelerationStructureKHRFunction() const noexcept { return m_vkCreateAccelerationStructureKHR; }
        PFN_vkDestroyAccelerationStructureKHR GetDestroyAccelerationStructureKHRFunction() const noexcept { return m_vkDestroyAccelerationStructureKHR; }
        PFN_vkGetAccelerationStructureBuildSizesKHR GetAccelerationStructureBuildSizesKHRFunction() const noexcept { return m_vkGetAccelerationStructureBuildSizesKHR; }
        PFN_vkBuildAccelerationStructuresKHR GetBuildAccelerationStructuresKHRFunction() const noexcept { return m_vkBuildAccelerationStructuresKHR; }
        PFN_vkCmdBuildAccelerationStructuresKHR GetCmdBuildAccelerationStructuresKHRFunction() const noexcept { return m_vkCmdBuildAccelerationStructuresKHR; }
        PFN_vkGetAccelerationStructureDeviceAddressKHR GetAccelerationStructureDeviceAddressKHRFunction() const noexcept { return m_vkGetAccelerationStructureDeviceAddressKHR; }

        inline size_t GetFrameNumber() const noexcept { return m_FrameCount; }
        inline size_t GetFrameIndex() const noexcept { return m_FrameCount % kMaxFramesInFlight; }

        PFN_vkSetDebugUtilsObjectNameEXT GetSetDebugUtilsObjectNameEXTFunction() const noexcept;
        void AddFenceCompletionCallback(VkFence fence, function_t<void()> continuation);
        uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

    private:
        void InitializeDevice(VkSurfaceKHR surface);
    };
}
