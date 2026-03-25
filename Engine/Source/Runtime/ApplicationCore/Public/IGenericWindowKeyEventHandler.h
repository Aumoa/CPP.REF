// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "KeyCode.h"

namespace Ayla
{
	interface IGenericWindowKeyEventHandler
	{
		virtual void OnKeyDown(KeyCode keyCode) = 0;
		virtual void OnKeyUp(KeyCode keyCode) = 0;
	};
}