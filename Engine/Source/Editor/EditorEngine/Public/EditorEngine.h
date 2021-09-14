// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class EDITORENGINE_API SEditorEngine : public SGameEngine
{
	GENERATED_BODY(SEditorEngine)

public:
	SEditorEngine();
	virtual ~SEditorEngine() override;
};