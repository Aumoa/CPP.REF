// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IEngineLoop.h"
#include "GenericPlatform/GenericWindow.h"
#include "Bootstrap/BootstrapTask.h"

class NGenericApplication;
class NSlateApplication;

class NEngineLoop : public IEngineLoop, public NonCopyable
{
private:
    std::unique_ptr<NSlateApplication> SlateApp;

public:
    NEngineLoop();
    virtual ~NEngineLoop() noexcept override;

    virtual void Init(NInitializeContext* InContext) override;
    virtual void Tick() override;
};