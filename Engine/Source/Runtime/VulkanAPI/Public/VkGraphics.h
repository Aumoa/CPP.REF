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
        VkDevice m_Device{ nullptr };

    public:
        VkGraphics(GenericApplication* app);

        virtual void InstallSwapChain(std::shared_ptr<GenericWindow> targetWindow) override;
    };
}