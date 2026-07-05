// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"
#include "BufferUsage.h"
#include "Rendering/ShaderCreationInfo.h"
#include "Graphics.gen.h"

namespace Ayla
{
    class GenericApplication;
    class GenericWindow;
    class GenericWindowSwapchainExtension;
    class CommandBuffer;
    class Buffer;
    class GraphicsRenderPipeline;
    class RaytracingRenderPipeline;
    class Shader;
    enum class ShaderType;

    ACLASS()
    class RENDERCORE_API Graphics : public Object
    {
        GENERATED_BODY()

    public:
        static constexpr size_t kMaxFramesInFlight = 2;
        static constexpr size_t kMaxSwapchainImages = 3;

    private:
        std::mutex m_PooledCommandBufferMutex;
        std::vector<SharedPtr<CommandBuffer>> m_PooledCommandBuffers;
        std::atomic<size_t> m_FrameNumber;

    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        virtual void Dispose() noexcept = 0;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept = 0;

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) = 0;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer() = 0;
        virtual SharedPtr<GraphicsRenderPipeline> CreateGeometryRenderPipeline(SharedPtr<Shader> shader) = 0;
        virtual SharedPtr<RaytracingRenderPipeline> CreateRaytracingRenderPipeline(SharedPtr<Shader> shader) = 0;
        virtual SharedPtr<Shader> CreateShader(ShaderCreationInfo shaderCreationInfo) = 0;
        virtual SharedPtr<Buffer> CreateUploadBuffer(size_t sizeInBytes) = 0;
        virtual SharedPtr<Buffer> CreateBuffer(std::span<const byte> data, size_t stride, BufferUsage usage) = 0;

        SharedPtr<CommandBuffer> GetPooledCommandBuffer();
        void ReleasePooledCommandBuffer(SharedPtr<CommandBuffer> cmd);

        virtual void BeginRenderFrame();
        virtual void EndRenderFrame();
        virtual void WaitForCompletion() = 0;

        inline size_t GetFrameIndex() const noexcept { return m_FrameNumber; }
        inline size_t GetPreviousFramePageIndex() const noexcept { return (m_FrameNumber - 1) % kMaxFramesInFlight; }
        inline size_t GetFramePageIndex() const noexcept { return m_FrameNumber % kMaxFramesInFlight; }
    };
}
