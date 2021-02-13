// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIRenderTarget.h"

#include "D3D12Minimal.h"

class D3D12RenderTarget : virtual public Object, virtual public IRHIRenderTarget
{
public:
	using Super = Object;
	using This = D3D12RenderTarget;

public:
	D3D12RenderTarget();
	~D3D12RenderTarget() override;

	virtual void BeginRender(ID3D12GraphicsCommandList* inCommandList) = 0;
	virtual void EndRender(ID3D12GraphicsCommandList* inCommandList) = 0;
};