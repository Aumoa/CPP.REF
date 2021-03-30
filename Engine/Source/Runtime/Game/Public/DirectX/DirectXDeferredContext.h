// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceContext.h"

class GAME_API DirectXDeferredContext : public DirectXDeviceContext
{
public:
	using Super = DirectXDeviceContext;

private:
	TComPtr<ID3D12CommandAllocator> swapAllocator;

public:
	DirectXDeferredContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type);
	~DirectXDeferredContext() override;

	virtual void BeginDraw();

	void DebugName_set(TRefPtr<String> value) override;
};