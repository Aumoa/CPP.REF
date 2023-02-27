// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"

class NGenericApplication;

class NEngineLoop : public IEngineLoop
{
private:
    std::unique_ptr<NGenericApplication> GenericApp;

public:
    struct NInitializeContext
    {
        std::unique_ptr<NGenericWindow> SplashWindow;
    };

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    std::unique_ptr<NInitializeContext> PreInit();

    virtual int32 GetExitCode() const override;
};