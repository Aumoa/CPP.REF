// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.gen.h"

namespace Ayla
{
    ACLASS()
    class APPLICATIONCORE_API GenericWindowExtension : public Object
    {
        GENERATED_BODY()

    protected:
        GenericWindowExtension() noexcept;

    public:
        virtual ~GenericWindowExtension() noexcept;
    };
}