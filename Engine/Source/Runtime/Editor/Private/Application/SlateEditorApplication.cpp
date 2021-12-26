// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateEditorApplication.h"
#include "Application/EditorWindow.h"

GENERATE_BODY(SSlateEditorApplication);

SSlateEditorApplication::SSlateEditorApplication() : Super()
{
}

SWindow* SSlateEditorApplication::CreateCoreWindow()
{
	return SNew(SEditorWindow);
}