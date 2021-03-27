// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "DirectX/DirectXMinimal.h"

class GAME_API RenderTarget : virtual public Object
{
public:
	using Super = Object;

private:
	uint32 width;
	uint32 height;

public:
	RenderTarget();
	~RenderTarget() override;

	virtual void BeginDraw(ID3D12GraphicsCommandList4* inCommandList) = 0;
	virtual void EndDraw(ID3D12GraphicsCommandList4* inCommandList) = 0;
	virtual void ResizeBuffers(uint32 width, uint32 height);

	vs_property_get_auto(uint32, Width, width);
	vs_property_get_auto(uint32, Height, height);
};