// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GraphicsRenderFeature.h"

namespace Ayla
{
    class LINUXAPI_API LinuxRenderFeature : public GraphicsRenderFeature
    {
    public:
        virtual bool HasGenericWindow() override;
        virtual std::shared_ptr<GenericWindow> CreateGenericWindow() override;
    };
}
