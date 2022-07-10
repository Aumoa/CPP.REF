// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "Delegates/MulticastDelegate.h"
#include "IPlatformWindow.generated.h"

SINTERFACE()
interface CORE_API IPlatformWindow : virtual public Object
{
	GENERATED_BODY()

	virtual void Show(bool bShow) = 0;
	virtual void Destroy() = 0;

	DECLARE_MULTICAST_DELEGATE(WindowDestroyedDelegate);
	WindowDestroyedDelegate WindowDestroyed;
};