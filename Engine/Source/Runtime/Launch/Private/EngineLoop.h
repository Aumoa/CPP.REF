// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"

class NGenericApplication;
class NSlateApplication;

class NEngineLoop : public IEngineLoop
{
    NEngineLoop(const NEngineLoop&) = delete;
    NEngineLoop(NEngineLoop&&) = delete;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    virtual void Init() override;
    virtual void Tick() override;

    void PreInit(String CmdArgs);
    void PostInit();

private:
    void PreInitPreStartupScreen(String CmdArgs);
    void PreInitPostStartupScreen();
};