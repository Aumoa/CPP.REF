// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class MAINFRAME_API SEditorPlayMenu : public SCompoundWidget
{
	GENERATED_SLATE_BODY(SEditorPlayMenu)

public:
	BEGIN_SLATE_ATTRIBUTE()
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

private:
	Task<std::shared_ptr<NRHITexture2D>> LoadTextureAsync(String InFilename);
};