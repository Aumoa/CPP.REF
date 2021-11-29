// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIImage.h"

interface IRHIBitmap : implements IRHIImage
{
	virtual Vector2N GetSize() = 0;
};