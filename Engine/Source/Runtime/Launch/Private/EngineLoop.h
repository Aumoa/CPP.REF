// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"

class NGenericApplication;
class NSlateApplication;
class SViewport;

class NEngineLoop : public IEngineLoop
{
    NEngineLoop(const NEngineLoop&) = delete;
    NEngineLoop(NEngineLoop&&) = delete;

private:
    RefPtr<AGameEngine> EngineInstance;
    std::shared_ptr<SViewport> GameViewport;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    virtual void Init(TSubclassOf<AGameEngine> InEngineClass) override;
    virtual void Shutdown() override;
    virtual void Tick(const std::vector<NGenericPlatformInputEvent>& InputEvents) override;

    void PreInit(String CmdArgs);
    void PostInit();

    inline std::shared_ptr<SViewport> GetGameViewport() const { return GameViewport; }

private:
    void PreInitPreStartupScreen(String CmdArgs);
    void PreInitPostStartupScreen();
};