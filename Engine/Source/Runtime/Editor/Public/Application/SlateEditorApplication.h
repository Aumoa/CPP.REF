// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Application/SlateApplication.h"

class EDITOR_API SSlateEditorApplication : public SSlateApplication
{
	GENERATED_BODY(SSlateEditorApplication)

public:
	SSlateEditorApplication();

protected:
	virtual SWindow* CreateCoreWindow() override;
};