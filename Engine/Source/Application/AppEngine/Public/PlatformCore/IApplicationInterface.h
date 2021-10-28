// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIDevice;

interface IApplicationInterface : implements SObject
{
	virtual std::wstring GetCommandLine() = 0;
	virtual void Start() = 0;
	virtual Vector2N GetViewportSize() = 0;

	virtual IRHIDevice* CreateDevice() = 0;

	DECLARE_MULTICAST_DELEGATE(IdleDelegate);
	IdleDelegate Idle;
	DECLARE_MULTICAST_DELEGATE(SizedDelegate, Vector2N);
	SizedDelegate Sized;
};