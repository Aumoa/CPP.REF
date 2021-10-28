// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/TickCalc.h"

interface IApplicationInterface;
interface IRHIDevice;
class SApplicationModule;
class SWindow;

class APPENGINE_API SAppEngine : implements SObject
{
	GENERATED_BODY(SAppEngine)

private:
	IRHIDevice* Device = nullptr;
	SApplicationModule* AppModule = nullptr;
	SWindow* CoreWindow = nullptr;
	TickCalc<> TickCalc;

public:
	SAppEngine();

	int32 GuardedMain(IApplicationInterface* Application);

private:
	void OnIdle();
	void OnSized(Vector2N Size);
};