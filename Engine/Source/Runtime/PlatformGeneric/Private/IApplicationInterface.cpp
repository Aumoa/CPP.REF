// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "IApplicationInterface.h"

using namespace ::libty;

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