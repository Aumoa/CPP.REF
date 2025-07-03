// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Graphics.h"
#include "VkCommon.h"

namespace Ayla
{
    class GenericApplication;

    class VULKANAPI_API VkGraphics : public Graphics
    {
    private:
        VkInstance m_Instance{ nullptr };

    public:
        VkGraphics(GenericApplication* app);
    };
}