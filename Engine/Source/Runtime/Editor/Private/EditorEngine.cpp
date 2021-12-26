// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EditorEngine.h"
#include "Application/SlateEditorApplication.h"

GENERATE_BODY(SEditorEngine);

SEditorEngine::SEditorEngine()
{
}

SSlateApplication* SEditorEngine::CreateSlateApplication()
{
	return gcnew SSlateEditorApplication();
}