// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "EditorEngine.generated.h"

SCLASS()
class GAME_API EditorEngine : public Engine
{
	GENERATED_BODY()

public:
	EditorEngine();

	virtual void Init() override;

protected:
	virtual void DispatchEngineTick(float elapsedSeconds) override;
};