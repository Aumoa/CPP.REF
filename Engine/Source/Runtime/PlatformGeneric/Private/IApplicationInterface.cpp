// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IApplicationInterface.h"

static IApplicationInterface* gApp;

IApplicationInterface::IApplicationInterface()
{
	check(gApp == nullptr);
	gApp = this;
}

IApplicationInterface& IApplicationInterface::Get()
{
	return *gApp;
}