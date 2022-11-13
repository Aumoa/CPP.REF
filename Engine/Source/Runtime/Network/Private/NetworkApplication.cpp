// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "NetworkApplication.h"

static constexpr LogCategory LogNet(TEXT("LogNet"));

NetworkApplication::NetworkApplication()
{
}

int32 NetworkApplication::Run()
{
	Log::Info(LogNet, TEXT("Content Directory: {}"), Environment::GetCurrentDirectory());
	Log::Info(LogNet, TEXT("Application startup."));
}