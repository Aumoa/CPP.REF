// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"
#include "IGenericWindowDestroyEventHandler.h"
#include "GenericMainWindowDefaultExt.gen.h"

namespace Ayla
{
    ACLASS()
    class APPLICATIONCORE_API GenericMainWindowDefaultExt : public GenericWindowExtension, public IGenericWindowDestroyEventHandler
    {
        GENERATED_BODY()

    public:
        GenericMainWindowDefaultExt();
        virtual ~GenericMainWindowDefaultExt() noexcept override;

        virtual void OnDestroy() override;
    };
}