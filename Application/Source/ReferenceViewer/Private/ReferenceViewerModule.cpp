// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ReferenceViewerModule.h"
#include "Widgets/CoreWindow.h"

SWindow* SReferenceViewerModule::CreateWindow()
{
	return SNew(SCoreWindow).Application(GetApp());
}

DEFINE_APPLICATION_MODULE(SReferenceViewerModule);