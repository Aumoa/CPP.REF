// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "LinuxFeatures.h"
#include "LinuxApplication.h"

namespace Ayla
{
    bool LinuxFeatures::HasGenericApplication()
    {
        return true;
    }

    std::shared_ptr<GenericApplication> LinuxFeatures::CreateGenericApplication()
    {
        return std::make_shared<LinuxApplication>();
    }
}
