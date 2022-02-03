// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModule.h"

class EDITOR_API SEditorModule : public SGameModule
{
	GENERATED_BODY(SEditorModule)

public:
	SEditorModule();

	virtual SGameEngine* CreateGameEngine() override;
};