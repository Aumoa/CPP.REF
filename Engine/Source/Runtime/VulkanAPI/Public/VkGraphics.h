// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
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
#if __cpp_lib_move_only_function
        using function_t = std::move_only_function<TBody>;
#else
        using function_t = std::function<TBody>;
#endif

    private:
        VkInstanceRef m_Instance;
        VkPhysicalDevice m_PhysicalDevice{ nullptr };
        VkDeviceRef m_Device;
        std::array<std::unique_ptr<VkCommandQueue>, 3> m_Queues;

        std::atomic<std::size_t> m_FrameCount = 0;

        std::mutex m_FenceCompletionMutex;
		std::map<VkFence, function_t<void()>> m_FenceCompletionCallbacks;

    public:
        ACONSTRUCTOR()
        VkGraphics();
        virtual ~VkGraphics() noexcept override;

        virtual void Dispose() noexcept override;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept override { return RenderFeatures::Vulkan; }

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow) override;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer_Implementation() override;  // VkCommandBuffer.cpp
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

        inline size_t GetFrameNumber() const noexcept { return m_FrameCount; }
        inline size_t GetFrameIndex() const noexcept { return m_FrameCount % kMaxFramesInFlight; }

        PFN_vkSetDebugUtilsObjectNameEXT GetSetDebugUtilsObjectNameEXTFunction() const noexcept;
        void AddFenceCompletionCallback(VkFence fence, function_t<void()> continuation);
    };
}