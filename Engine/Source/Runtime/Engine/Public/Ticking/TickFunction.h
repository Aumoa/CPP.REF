// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ENGINE_API NTickFunction
{
protected:
	NTickFunction();

	virtual void ExecuteTick(const TimeSpan& InDeltaTime) = 0;

	void SetTickFunctionEnable(bool bEnabled);
	bool IsTickFunctionEnabled() const;
	bool IsTickFunctionRegistered() const;
};