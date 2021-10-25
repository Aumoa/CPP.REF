// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Application/SlateApplication.h"
#include "Widgets/Window.h"

SSlateApplication::SSlateApplication() : Super()
{
}

void SSlateApplication::InitWindow(SLocalPlayer* InLocalPlayer, IFrameworkView* InFrameworkView)
{
	LocalPlayer = InLocalPlayer;
	(CoreWindow = SNew(SWindow))->InitViewport(InFrameworkView);
}