// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"
#include "EditorEngine.generated.h"

ACLASS()
class COREED_API AEditorEngine : public AGameEngine
{
	GENERATED_BODY()

public:
	AEditorEngine();
	virtual ~AEditorEngine() noexcept override;
};