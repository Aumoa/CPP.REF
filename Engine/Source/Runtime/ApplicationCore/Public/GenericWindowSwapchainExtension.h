// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"

namespace Ayla
{
    class APPLICATIONCORE_API GenericWindowSwapchainExtension : public GenericWindowExtension
    {
    protected:
        GenericWindowSwapchainExtension() noexcept;

    public:
        virtual ~GenericWindowSwapchainExtension() noexcept;

        virtual void Present() = 0;
    };
}