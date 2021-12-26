// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameEngine.h"

class EDITOR_API SEditorEngine : public SGameEngine
{
	GENERATED_BODY(SEditorEngine)

public:
	SEditorEngine();

protected:
	virtual SSlateApplication* CreateSlateApplication() override;
};