// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIGraphics.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Graphics : public NRHIGraphics
{
	ComPtr<IDXGIFactory7> DXGIFactory;
	ComPtr<IDXGIAdapter1> CurrentAdapter;
	ComPtr<ID3D12Device> Device;

public:
	ND3D12Graphics();
	virtual ~ND3D12Graphics() noexcept override;

	virtual void Init() override;
	virtual std::shared_ptr<NRHICommandQueue> CreateCommandQueue() override;

	ID3D12Device* GetDevice() const;

public:
	static std::unique_ptr<NRHIGraphics> GenerateGraphics();
};

#endif