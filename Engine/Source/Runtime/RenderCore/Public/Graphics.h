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

    ACLASS()
    class RENDERCORE_API Graphics : public Object
    {
        GENERATED_BODY()

    public:
        static constexpr size_t kMaxFramesInFlight = 2;
        static constexpr size_t kMaxSwapchainImages = 3;

    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        virtual void Dispose() noexcept = 0;
        virtual RenderFeatures GetCurrentRenderFeature() noexcept = 0;

        AFUNCTION()
        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) APURE;
        AFUNCTION()
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer() APURE;
        AFUNCTION()
        virtual SharedPtr<Buffer> CreateBuffer(BufferUsage usage) APURE;

        virtual void BeginRenderFrame() = 0;
        virtual void EndRenderFrame() = 0;
        virtual void WaitForCompletion() = 0;
    };
}