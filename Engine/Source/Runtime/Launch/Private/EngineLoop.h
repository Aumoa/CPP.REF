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

private:
    RefPtr<AGameEngine> EngineInstance;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    virtual void Init(TSubclassOf<AGameEngine> InEngineClass) override;
    virtual void Shutdown() override;
    virtual void Tick() override;

    void PreInit(String CmdArgs);
    void PostInit();

private:
    void PreInitPreStartupScreen(String CmdArgs);
    void PreInitPostStartupScreen();
};