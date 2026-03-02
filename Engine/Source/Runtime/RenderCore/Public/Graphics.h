// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"
#include "BufferUsage.h"
#include "Graphics.gen.h"

namespace Ayla
{
    class GenericApplication;
    class GenericWindow;
    class GenericWindowSwapchainExtension;
    class CommandBuffer;
    class Buffer;
    class RenderPipeline;
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

    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        virtual void Dispose() noexcept = 0;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept = 0;

        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) = 0;
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer() = 0;
        virtual SharedPtr<RenderPipeline> CreateRaytracingRenderPipeline(std::vector<SharedPtr<Shader>> shaders) = 0;
        virtual SharedPtr<Shader> CreateShader(std::vector<byte> bytecode, ShaderType type, String entrypointName) = 0;

        SharedPtr<CommandBuffer> GetPooledCommandBuffer();
        void ReleasePooledCommandBuffer(SharedPtr<CommandBuffer> cmd);

        virtual void BeginRenderFrame() = 0;
        virtual void EndRenderFrame() = 0;
        virtual void WaitForCompletion() = 0;
    };
}