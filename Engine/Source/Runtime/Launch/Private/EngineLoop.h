// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"
#include "Bootstrap/BootstrapTask.h"

class NGenericApplication;
class NSlateApplication;

class NEngineLoop : public IEngineLoop
{
    NEngineLoop(const NEngineLoop&) = delete;
    NEngineLoop(NEngineLoop&&) = delete;

private:
    std::unique_ptr<NSlateApplication> SlateApp;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    virtual void Init(NInitializeContext* InContext) override;
    virtual void Tick() override;

private:
    void InitScripts(NInitializeContext* InContext);
    void InitRHIs(NInitializeContext* InContext);
};