// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "SubclassOf.h"
#include "Diagnostics/PerformanceTimer.h"
#include "Misc/CommandLineBuilder.h"
#include "PlatformApplication.generated.h"

interface IPlatformWindow;

SCLASS()
class CORE_API PlatformApplication : virtual public Object
{
	GENERATED_BODY()

private:
	PerformanceTimer _gcTimer;
	int32 _platformExitCode = -1;

protected:
	PlatformApplication();

public:
	virtual int32 Startup(const CommandLineBuilder& builder) = 0;

	void StartApplicationLoop();
	virtual void RequestExit(int32 exitCode);
	virtual void Tick();

	int32 GetPlatformExitCode();

	virtual IPlatformWindow* CreateWindow(const String& title);

public:
	static int32 GuardedMain(SubclassOf<PlatformApplication> classOf, const CommandLineBuilder& builder);
};