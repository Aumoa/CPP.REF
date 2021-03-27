// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXDeviceBundle;

class GAME_API DirectXDeviceContext : public DirectXDeviceResource
{
	friend class DirectXCommandQueue;

public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;
	D3D12_COMMAND_LIST_TYPE commandType;

	TComPtr<ID3D12CommandAllocator> commandAllocator;
	TComPtr<ID3D12GraphicsCommandList4> commandList;
	bool bHasBegunDraw : 1;

	std::vector<TRefPtr<Object>> pendingReferences;

public:
	DirectXDeviceContext(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type);
	~DirectXDeviceContext() override;

	virtual void BeginDraw(ID3D12PipelineState* initialPipeline = nullptr);
	virtual void EndDraw();

	ID3D12GraphicsCommandList4* GetCommandList() const;
	D3D12_COMMAND_LIST_TYPE GetCommandListType() const;
	vs_property_get_auto(bool, HasBegunDraw, bHasBegunDraw);

	void AddPendingReference(TRefPtr<Object>&& pendingReference);
	void AddPendingReference(TComPtr<IUnknown>&& pendingReference);

protected:
	void SwapCommandAllocator(TComPtr<ID3D12CommandAllocator>& swapTarget);
};