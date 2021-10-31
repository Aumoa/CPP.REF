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

	Vector2N _requiredSize;
	ComPtr<ID3D12Resource> _resource;
	ComPtr<ID3D11Resource> _wrapped;
	ComPtr<ID2D1Bitmap1> _bitmap;
	bool _bNeedRender = true;

public:
	SDWriteTextLayout(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDWriteTextLayout> layout);

	virtual Vector2 GetMaxSize() override;

private:
	void AllocateBuffer(const Vector2N& required);
	static constexpr int32 AlignAs128(int32 required);
};