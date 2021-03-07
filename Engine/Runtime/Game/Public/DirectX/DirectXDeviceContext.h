// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXDeviceBundle;

class GAME_API DirectXDeviceContext : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	TComPtr<ID3D12CommandAllocator> commandAllocator;
	TComPtr<ID3D12GraphicsCommandList4> commandList;

public:
	DirectXDeviceContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type);
	~DirectXDeviceContext() override;

	virtual void BeginDraw();
	virtual void EndDraw();

	ID3D12GraphicsCommandList4* GetCommandList() const;

protected:
	void SwapCommandAllocator(TComPtr<ID3D12CommandAllocator>& swapTarget);
};