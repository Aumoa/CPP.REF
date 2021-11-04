// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHITextLayout.h"

class SDWriteTextLayout : public SD3D12DeviceChild, implements IRHITextLayout
{
	GENERATED_BODY(SDWriteTextLayout)

private:
	ComPtr<IDWriteTextLayout> _layout;

public:
	SDWriteTextLayout(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDWriteTextLayout> layout);

	virtual Vector2 GetMaxSize() override;

public:
	DECLARE_GETTER(IDWriteTextLayout, _layout);
};