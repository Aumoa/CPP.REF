// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ApplicationModule.h"
#include "Widgets/Viewport.h"
#include "PlatformCore/IApplicationInterface.h"

SApplicationModule::SApplicationModule()
{
}

SWindow* SApplicationModule::CreateWindow()
{
	return SNew(SViewport).Application(Cast<IApplicationInterface>(GetOuter()));
}