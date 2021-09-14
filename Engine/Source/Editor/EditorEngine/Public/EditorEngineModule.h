// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class EDITORENGINE_API SEditorEngineModule : public SGameModule
{
	GENERATED_BODY(SEditorEngineModule)

public:
	SEditorEngineModule();
	virtual ~SEditorEngineModule() override;

	virtual SGameEngine* CreateGameEngine() override;
};