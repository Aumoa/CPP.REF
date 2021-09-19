// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EditorEngine.h"
#include "IFrameworkView.h"

SEditorEngine::SEditorEngine() : Super()
{
}

SEditorEngine::~SEditorEngine()
{
}

bool SEditorEngine::InitEngine()
{
	if (!Super::InitEngine())
	{
		return false;
	}

	return true;
}

void SEditorEngine::SetupFrameworkView(IFrameworkView* frameworkView)
{
	Super::SetupFrameworkView(frameworkView);

	frameworkView->SetTickMode(IFrameworkView::ETickMode::Ontime);
}