// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "Subsystem.generated.h"

class NSubsystemCollection;

ACLASS()
class ENGINE_API ASubsystem : public AObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(NSubsystemCollection& Collection) = 0;
	virtual void PostInitialize() = 0;
	virtual void Deinitialize() = 0;
};