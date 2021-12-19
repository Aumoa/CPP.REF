// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIBrush.h"

interface IRHISolidColorBrush : implements IRHIBrush
{
	GENERATED_INTERFACE_BODY(IRHISolidColorBrush)

	virtual void SetColor(const Color& InColor) = 0;
	virtual Color GetColor() = 0;
};