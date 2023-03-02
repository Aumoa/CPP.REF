// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"
#include "Bootstrap/BootstrapTask.h"

class NGenericApplication;

class NEngineLoop : public IEngineLoop
{
private:
    std::unique_ptr<NGenericApplication> GenericApp;

public:
    struct NInitializeContext
    {
        std::optional<NBootstrapTask> GraphicsTask;
    };

    std::unique_ptr<NInitializeContext> Context;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    NInitializeContext* PreInit();

    virtual int32 GetExitCode() const override;
    virtual void Init() override;
};