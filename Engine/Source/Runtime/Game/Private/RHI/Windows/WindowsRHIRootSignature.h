// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIRootSignature.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;
struct RHIRootSignatureDesc;

class WindowsRHIRootSignature : public RHIRootSignature
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12RootSignature> _signature;

protected:
	WindowsRHIRootSignature(std::shared_ptr<WindowsRHIDevice> device, const RHIRootSignatureDesc& desc);

public:
	virtual ~WindowsRHIRootSignature() noexcept override;

	DECLGET(Signature, _signature);
};

inline ID3D12RootSignature* WinGetr(RHIRootSignature* sig)
{
	if (sig)
	{
		return static_cast<WindowsRHIRootSignature*>(sig)->GetSignature().Get();
	}
	else
	{
		return nullptr;
	}
}

#endif