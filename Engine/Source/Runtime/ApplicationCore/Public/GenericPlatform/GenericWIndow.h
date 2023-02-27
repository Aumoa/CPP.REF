// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericWindowDefinition.h"

class APPLICATIONCORE_API NGenericWindow
{
protected:
    NGenericWindow();

public:
    virtual ~NGenericWindow() noexcept;

    virtual NGenericWindowDefinition GetDefinition() const = 0;
    virtual void* GetOSWindowHandle() const = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
};