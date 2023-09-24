// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SViewport.h"

class MAINFRAME_API SEditorViewport : public SViewport
{
	GENERATED_SLATE_BODY(SEditorViewport)

public:
	SEditorViewport();

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

private:
	Task<std::shared_ptr<NRHITexture2D>> LoadTextureAsync();
};