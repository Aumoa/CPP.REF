// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputComponent.gen.h"

SCLASS()
class GAME_API InputComponent : public ActorComponent
{
	GENERATED_BODY()

protected:
	virtual void RegisterComponent() override;
	virtual void UnregisterComponent() override;
};