// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXCommandQueue;
class WinEvent;

class GAME_API DirectXAutoFence : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	TComPtr<ID3D12Fence> fence;
	uint64 fenceValue;
	bool bBusyloop : 1;
	TRefPtr<WinEvent> winEvent;

public:
	DirectXAutoFence(DirectXDeviceBundle* deviceBundle, bool bBusyloop = false);
	~DirectXAutoFence() override;

	uint64 GetCompletedValue() const;
	uint64 AcceptSignal(DirectXCommandQueue* queue);
	bool Wait(std::optional<Seconds> timeout = std::nullopt) const;

private:
	bool Wait_Busyloop(std::optional<Seconds> timeout) const;
	bool Wait_Event(std::optional<Seconds> timeout) const;
};