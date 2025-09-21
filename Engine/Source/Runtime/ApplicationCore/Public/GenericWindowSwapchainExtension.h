// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"
#include "GenericWindowSwapchainExtension.gen.h"

namespace Ayla
{
    ACLASS()
    class APPLICATIONCORE_API GenericWindowSwapchainExtension : public GenericWindowExtension
    {
        GENERATED_BODY()

    protected:
        GenericWindowSwapchainExtension() noexcept;

    public:
        virtual ~GenericWindowSwapchainExtension() noexcept;

        virtual void Present() = 0;
        virtual void Destroy() = 0;
    };
}