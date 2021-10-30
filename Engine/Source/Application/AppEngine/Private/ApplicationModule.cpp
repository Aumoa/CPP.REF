// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ApplicationModule.h"
#include "Widgets/Viewport.h"
#include "PlatformCore/IApplicationInterface.h"

SApplicationModule::SApplicationModule()
{
}

void SApplicationModule::Init(IApplicationInterface* App)
{
	this->App = App;
}

void SApplicationModule::Start()
{
}

SWindow* SApplicationModule::CreateWindow()
{
	return SNew(SViewport).Application(Cast<IApplicationInterface>(GetOuter()));
}

IApplicationInterface* SApplicationModule::GetApp()
{
	return App;
}