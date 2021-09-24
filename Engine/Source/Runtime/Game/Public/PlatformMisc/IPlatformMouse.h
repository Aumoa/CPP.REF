// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameStructures.h"

struct IFrameworkView;

struct GAME_API IPlatformMouse : implements SObject
{
	virtual MouseState GetState() const = 0;
	virtual void ResetScrollWheelValue() = 0;
	virtual void SetMode(EMousePositionMode mode) = 0;
	virtual bool IsConnected() const = 0;
	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool bVisible) = 0;
	virtual void SetWindow(IFrameworkView* frameworkView) = 0;

	static IPlatformMouse& Get();
};