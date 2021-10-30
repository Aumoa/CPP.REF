// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Layout.h"

interface IRHIBrush : implements SObject
{
	virtual void SetOpacity(float InOpacity) = 0;
	virtual float GetOpacity() = 0;
	virtual void SetTransform(const SlateRenderTransform& InTransform) = 0;
	virtual SlateRenderTransform GetTransform() = 0;
};