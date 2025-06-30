// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlatformFeatures.h"

namespace Ayla
{
    class LINUXAPI_API LinuxFeatures : public PlatformFeatures
    {
    public:
        virtual bool HasGenericApplication() override;
        virtual std::shared_ptr<GenericApplication> CreateGenericApplication() override;
    };
}
