// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D2D1Brush.h"
#include "RHI/IRHISolidColorBrush.h"

class DIRECTX_API SD2D1SolidColorBrush : public SD2D1Brush, implements IRHISolidColorBrush
{
	GENERATED_BODY(SD2D1SolidColorBrush)

private:
	ID2D1SolidColorBrush* Brush = nullptr;

public:
	SD2D1SolidColorBrush(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID2D1SolidColorBrush* InBrush);

	virtual void SetColor(const Color& InColor) override;
	virtual Color GetColor() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};