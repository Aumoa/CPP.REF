// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHITextLayout.h"

class SDWriteTextLayout : public SDXGIFactoryChild, implements IRHITextLayout
{
	GENERATED_BODY(SDWriteTextLayout)

private:
	ComPtr<IDWriteTextLayout> _layout;

public:
	SDWriteTextLayout(SDXGIFactory* factory, ComPtr<IDWriteTextLayout> layout);

	virtual void SetMaxSize(const Vector2& InSize) override;
	virtual Vector2 GetMaxSize() override;
	virtual Vector2 GetDesiredSize() override;

public:
	DECLARE_GETTER(IDWriteTextLayout, _layout);
};