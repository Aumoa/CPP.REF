// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "LinuxRenderFeature.h"

namespace Ayla
{
    bool LinuxRenderFeature::HasGenericWindow()
    {
        return true;
    }

    std::shared_ptr<GenericWindow> LinuxRenderFeature::CreateGenericWindow()
    {
    }
}
