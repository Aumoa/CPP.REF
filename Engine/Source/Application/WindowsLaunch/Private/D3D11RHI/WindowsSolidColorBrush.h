// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowsBrush.h"
#include "RHI/IRHISolidColorBrush.h"

class SWindowsSolidColorBrush : public SWindowsBrush, implements IRHISolidColorBrush
{
	GENERATED_BODY(SWindowsSolidColorBrush)

private:
	ID2D1SolidColorBrush* SolidBrush = nullptr;

public:
	SWindowsSolidColorBrush(ID2D1SolidColorBrush* InBrushObject);

	virtual void SetColor(const Color& InColor) override;
	virtual Color GetColor() override;
};