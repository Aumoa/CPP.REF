// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"

namespace Ayla
{
    class GenericApplication;
    class GenericWindow;
    class GenericWindowSwapchainExtension;

    class RENDERCORE_API Graphics
    {
    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        virtual std::shared_ptr<GenericWindowSwapchainExtension> InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow) = 0;
        virtual void BeginRenderThread() = 0;
        virtual void EndRenderThread() = 0;

        static std::shared_ptr<Graphics> CreateGraphics(RenderFeatures api);
    };
}

#define NAMEOF_CREATE_GRAPHICS TEXT("CreateGraphics")

#define DEFINE_CREATE_GRAPHICS(ClassName) \
extern "C" \
{ \
    PLATFORM_SHARED_EXPORT auto CreateGraphics() \
    { \
        return (::Ayla::Graphics*)(new ClassName()); \
    } \
}
