// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformInputEvent.h"

class GameEngine;

class IEngineLoop
{
protected:
    IEngineLoop()
    {
    }

public:
    virtual ~IEngineLoop() noexcept
    {
    }

    virtual void Init(std::shared_ptr<GameEngine> InEngine) = 0;
    virtual void Shutdown() = 0;
    virtual void Tick(const std::vector<NGenericPlatformInputEvent>& InputEvents) = 0;
};