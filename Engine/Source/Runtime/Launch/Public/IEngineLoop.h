// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bootstrap/BootstrapTask.h"

class IEngineLoop
{
protected:
    IEngineLoop() = default;

public:
    struct NInitializeContext
    {
        std::optional<NBootstrapTask> ScriptsTask;
        std::optional<NBootstrapTask> GraphicsTask;
    };

public:
    virtual ~IEngineLoop() noexcept = default;

    virtual void Init(NInitializeContext* InContext) = 0;
    virtual void Tick() = 0;
};