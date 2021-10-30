// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/IRHIStrokeStyle.h"
#include "WindowsIncludes.h"

class SWindowsStrokeStyle : implements SObject, implements IRHIStrokeStyle
{
	GENERATED_BODY(SWindowsStrokeStyle)

private:
	ComPtr<ID2D1StrokeStyle> Style;

public:
	SWindowsStrokeStyle(ID2D1StrokeStyle* InStyle);
	
	virtual RHIStrokeStyleDesc GetDesc() override;
	virtual std::vector<float> GetDashes() override;

	ID2D1StrokeStyle* GetStyle();
};