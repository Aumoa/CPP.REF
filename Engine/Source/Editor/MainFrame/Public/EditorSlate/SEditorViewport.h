// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SViewport.h"

class MAINFRAME_API SEditorViewport : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SEditorViewport)

public:
	SEditorViewport();

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};