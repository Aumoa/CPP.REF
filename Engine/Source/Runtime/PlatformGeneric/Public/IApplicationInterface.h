// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIFactory;
interface IPlatformKeyboard;
interface IPlatformMouse;

interface IApplicationInterface : implements SObject
{
	enum class ETickMode
	{
		Realtime,
		Ontime
	};

	virtual void Start() = 0;
	virtual Vector2N GetViewportSize() = 0;

	virtual void SetTickMode(ETickMode InTickMode) = 0;
	virtual ETickMode GetTickMode() = 0;

	virtual void SetTitle(std::wstring_view InTitle) = 0;
	virtual std::wstring GetTitle() = 0;

	virtual IRHIFactory* CreateFactory() = 0;
	virtual IPlatformKeyboard& GetPlatformKeyboard() = 0;
	virtual IPlatformMouse& GetPlatformMouse() = 0;

	DECLARE_MULTICAST_DELEGATE(IdleDelegate);
	IdleDelegate Idle;
	DECLARE_MULTICAST_DELEGATE(SizedDelegate, Vector2N);
	SizedDelegate Sized;

	PLATFORMGENERIC_API IApplicationInterface();
	PLATFORMGENERIC_API static IApplicationInterface& Get();
};