// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "COM/COMMinimal.h"
#include <dxgiformat.h>

interface IDXGIAdapter1;
interface IDXGIFactory2;
interface IDXGISwapChain4;
interface ID3D12Device5;
interface ID3D12DeviceChild;
interface ID3D12CommandQueue;
interface ID3D12CommandAllocator;
interface ID3D12GraphicsCommandList4;

#pragma warning(push)
#pragma warning(disable: 4369)

enum D3D12_COMMAND_LIST_TYPE : int;

#pragma warning(pop)