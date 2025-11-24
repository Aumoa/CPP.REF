// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"
#include "Graphics.gen.h"

namespace Ayla
{
    class GenericApplication;
    class GenericWindow;
    class GenericWindowSwapchainExtension;
    class CommandBuffer;
    class SceneView;

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

        AFUNCTION()
        virtual SharedPtr<GenericWindowSwapchainExtension> InstallSwapChain(SharedPtr<GenericWindow> targetWindow) APURE;
        AFUNCTION()
        virtual void BeginRenderFrame() APURE;
        AFUNCTION()
        virtual void EndRenderFrame() APURE;
        AFUNCTION()
        virtual SharedPtr<CommandBuffer> CreateCommandBuffer() APURE;
    };
}