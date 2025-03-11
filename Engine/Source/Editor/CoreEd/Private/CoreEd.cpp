// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "CoreEd.h"
#include "EditorEngine.h"
#include "Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "EditorSlate/SEditorViewport.h"

void NCoreEd::EditorInit(IEngineLoop& Loop)
{
	std::shared_ptr<SViewport> GameViewport = std::static_pointer_cast<SViewport>(NSlateApplication::Get().GetCoreWindow().GetContent());
	NSlateApplication::Get().GetCoreWindow().SetContent(SNew(SEditorViewport)
		+SEditorViewport::NSlot()
		.VAlignment(EVerticalAlignment::Fill)
		.HAlignment(EHorizontalAlignment::Fill)
		[
			GameViewport
		]
	);

	Loop.Init(std::make_shared<EditorEngine>());
}