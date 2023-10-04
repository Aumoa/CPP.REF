// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem.h"
#include "EngineSubsystem.generated.h"

ACLASS()
class ENGINE_API AEngineSubsystem : public ASubsystem
{
	GENERATED_BODY()

public:
	AEngineSubsystem();

	virtual void Initialize([[maybe_unused]] NSubsystemCollection& Collection) override {}
	virtual void PostInitialize() override {}
	virtual void Deinitialize() override {}
};