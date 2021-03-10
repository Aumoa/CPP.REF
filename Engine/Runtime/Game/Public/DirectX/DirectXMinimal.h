// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "COM/COMMinimal.h"
#include <dxgiformat.h>

interface IUnknown;
interface IDXGIAdapter1;
interface IDXGIFactory2;
interface IDXGISwapChain4;
interface ID3D12Device5;
interface ID3D12Fence;
interface ID3D12DeviceChild;
interface ID3D12CommandQueue;
interface ID3D12CommandAllocator;
interface ID3D12GraphicsCommandList4;
interface ID3D12DescriptorHeap;
interface ID3D12Resource;
interface ID3D12PipelineState;
interface ID3D12RootSignature;

#pragma warning(push)
#pragma warning(disable: 4369)

enum D3D12_COMMAND_LIST_TYPE : int;
enum D3D12_DESCRIPTOR_HEAP_TYPE : int;
enum D3D12_RESOURCE_STATES : int;
enum D3D12_RESOURCE_FLAGS : int;

#pragma warning(pop)

struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_GPU_DESCRIPTOR_HANDLE;
struct D3D12_RENDER_TARGET_VIEW_DESC;
struct D3D12_DEPTH_STENCIL_VIEW_DESC;
struct D3D12_CLEAR_VALUE;
struct D3D12_SHADER_RESOURCE_VIEW_DESC;
struct D3D12_UNORDERED_ACCESS_VIEW_DESC;
struct D3D12_RAYTRACING_INSTANCE_DESC;