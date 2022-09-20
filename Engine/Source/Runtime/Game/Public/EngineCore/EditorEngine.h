// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "EditorEngine.gen.h"

SCLASS()
class GAME_API EditorEngine : public Engine
{
	GENERATED_BODY()

public:
	EditorEngine();

	virtual void Init() override;
};