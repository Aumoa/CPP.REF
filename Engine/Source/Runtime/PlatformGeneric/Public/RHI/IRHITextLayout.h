// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIFactoryChild.h"

interface IRHITextLayout : implements IRHIFactoryChild
{
	virtual void SetMaxSize(const Vector2& InSize) = 0;
	virtual Vector2 GetMaxSize() = 0;
	virtual Vector2 GetDesiredSize() = 0;
};