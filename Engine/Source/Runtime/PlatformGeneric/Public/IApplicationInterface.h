// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

interface IRHIFactory;
interface IPlatformKeyboard;
interface IPlatformMouse;
interface IPlatformImageLoader;

interface IApplicationInterface : implements SObject
{
	GENERATED_INTERFACE_BODY(IApplicationInterface)

	enum class ETickMode
	{
		Realtime,
		Ontime
	};

	virtual void Start() = 0;

	virtual Vector2N GetViewportSize() = 0;
	virtual float GetDpi() = 0;
	virtual void* GetWindowHandle() = 0;

	virtual void SetTickMode(ETickMode InTickMode) = 0;
	virtual ETickMode GetTickMode() = 0;
	virtual void AddRealtimeDemander(SObject* InObject) = 0;
	virtual void RemoveRealtimeDemander(SObject* InObject) = 0;

	virtual void SetTitle(std::wstring_view InTitle) = 0;
	virtual std::wstring GetTitle() = 0;

	virtual IRHIFactory* GetFactory() = 0;
	virtual IPlatformKeyboard& GetPlatformKeyboard() = 0;
	virtual IPlatformMouse& GetPlatformMouse() = 0;
	virtual IPlatformImageLoader& GetPlatformImageLoader() = 0;

	DECLARE_MULTICAST_DELEGATE(IdleDelegate, ETickMode);
	IdleDelegate Idle;
	DECLARE_MULTICAST_DELEGATE(SizedDelegate, Vector2N);
	SizedDelegate Sized;

	PLATFORMGENERIC_API IApplicationInterface();
	PLATFORMGENERIC_API static IApplicationInterface& Get();
};