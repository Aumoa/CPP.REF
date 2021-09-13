// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct IWindowView : virtual public SObject
{
	/// <summary>
	/// Get window handle that platform specialized.
	/// </summary>
	virtual void* GetWindowHandle() const = 0;
};