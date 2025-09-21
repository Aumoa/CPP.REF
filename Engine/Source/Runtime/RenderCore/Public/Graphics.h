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

    ACLASS()
    class RENDERCORE_API Graphics : public Object
    {
        GENERATED_BODY()

    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        AFUNCTION()
        virtual std::shared_ptr<GenericWindowSwapchainExtension> InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow) APURE;
        virtual void BeginRenderThread() = 0;
        virtual void EndRenderThread() = 0;
    };
}