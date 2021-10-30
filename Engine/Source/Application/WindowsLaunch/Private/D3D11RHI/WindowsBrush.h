// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIBrush.h"
#include "WindowsIncludes.h"

class SWindowsBrush : implements SObject, implements IRHIBrush
{
	GENERATED_BODY(SWindowsBrush)

private:
	ComPtr<ID2D1Brush> BrushObject;

public:
	SWindowsBrush(ID2D1Brush* InBrush);

	virtual void SetOpacity(float InOpacity) override;
	virtual float GetOpacity() override;
	virtual void SetTransform(const SlateRenderTransform& InTransform) override;
	virtual SlateRenderTransform GetTransform() override;

	ID2D1Brush* GetBrush() { return BrushObject.Get(); }
};