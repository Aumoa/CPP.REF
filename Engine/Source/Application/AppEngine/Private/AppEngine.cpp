// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "AppEngine.h"
#include "IFrameworkView.h"

SAppEngine::SAppEngine() : Super()
{
}

SAppEngine::~SAppEngine()
{
}

bool SAppEngine::InitEngine()
{
	if (!Super::InitEngine())
	{
		return false;
	}

	return true;
}

void SAppEngine::SetupFrameworkView(IFrameworkView* frameworkView)
{
	Super::SetupFrameworkView(frameworkView);

	frameworkView->SetTickMode(IFrameworkView::ETickMode::Ontime);
}