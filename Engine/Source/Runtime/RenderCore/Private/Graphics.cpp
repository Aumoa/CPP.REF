// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Graphics.h"
#include "Platform/DynamicLibrary.h"
#include "GenericApplication.h"

namespace Ayla
{
    Graphics::Graphics()
    {
    }

    Graphics::~Graphics() noexcept
    {
    }

    std::shared_ptr<Graphics> Graphics::CreateGraphics(RenderFeatures api)
    {
        std::optional<DynamicLibrary> dl;

        switch (api)
        {
            case RenderFeatures::Vulkan:
                dl = DynamicLibrary(TEXT("VulkanAPI"));
                break;
            default:
                throw ArgumentException(TEXT("api"));
        }

        auto allocator = dl->LoadFunction<Graphics*>(NAMEOF_CREATE_GRAPHICS);
        if (allocator == nullptr)
        {
            throw InvalidOperationException(NAMEOF_CREATE_GRAPHICS + TEXT(" does not declared in ") + dl->GetName());
        }

        auto* gptr = allocator();
        dl->Detach();
        return std::shared_ptr<Graphics>{ gptr };
    }
}