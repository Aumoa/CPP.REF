// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RenderFeatures.h"

namespace Ayla
{
    class GenericApplication;

    class RENDERCORE_API Graphics
    {
    protected:
        Graphics();

    public:
        virtual ~Graphics() noexcept;

        static std::shared_ptr<Graphics> CreateGraphics(RenderFeatures api, GenericApplication* app);
    };
}

#define NAMEOF_CREATE_GRAPHICS TEXT("CreateGraphics")

#define DEFINE_CREATE_GRAPHICS(ClassName) \
extern "C" \
{ \
    PLATFORM_SHARED_EXPORT auto CreateGraphics(::Ayla::GenericApplication* app) \
    { \
        return (::Ayla::Graphics*)(new ClassName(app)); \
    } \
}
