// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericWindowExtension.h"
#include "IGenericWindowDestroyEventHandler.h"

namespace Ayla
{
    class GenericMainWindowDefaultExt : public GenericWindowExtension, public IGenericWindowDestroyEventHandler
    {
    public:
        GenericMainWindowDefaultExt();
        virtual ~GenericMainWindowDefaultExt() noexcept override;

        virtual void OnDestroy() override;
    };
}