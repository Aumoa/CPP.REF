// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bootstrap/BootstrapTask.h"

class IEngineLoop
{
protected:
    IEngineLoop() = default;

public:
    virtual ~IEngineLoop() noexcept = default;

    virtual int32 GetExitCode() const = 0;
    virtual void Init() = 0;
};