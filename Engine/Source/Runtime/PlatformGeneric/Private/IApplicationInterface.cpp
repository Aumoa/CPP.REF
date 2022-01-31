// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IApplicationInterface.h"

static IApplicationInterface* gApp;

IApplicationInterface::IApplicationInterface()
{
	checkf(gApp == nullptr, L"Application is registered as duplicate.");
	gApp = this;
}

IApplicationInterface& IApplicationInterface::Get()
{
	return *gApp;
}