// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "SubclassOf.h"
#include "GenericPlatform/GenericPlatformInputEvent.h"

class AGameEngine;

class ENGINE_API IEngineLoop
{
protected:
    IEngineLoop();

public:
    virtual ~IEngineLoop() noexcept;

    virtual void Init(TSubclassOf<AGameEngine> InEngineClass) = 0;
    virtual void Shutdown() = 0;
    virtual void Tick(const std::vector<NGenericPlatformInputEvent>& InputEvents) = 0;
};