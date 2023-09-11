// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreEd.h"
#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "EditorSlate/SEditorViewport.h"

void NCoreEd::EditorInit(IEngineLoop& Loop)
{
	Loop.Init();
	NSlateApplication::Get().GetCoreWindow().AddViewport(
		SNew(SEditorViewport)
	);
}