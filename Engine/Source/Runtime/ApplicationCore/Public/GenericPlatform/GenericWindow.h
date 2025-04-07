// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"
#include "GenericWindow.gen.h"

namespace Ayla
{
    ACLASS()
    class APPLICATIONCORE_API GenericWindow : public Object
    {
        GENERATED_BODY()

    protected:
        GenericWindow();

    public:
        virtual GenericWindowDefinition GetDefinition() const = 0;
        virtual void* GetOSWindowHandle() const = 0;
        virtual void Show() = 0;
        virtual void Hide() = 0;
        virtual Vector2N GetSize() const = 0;
    };
}