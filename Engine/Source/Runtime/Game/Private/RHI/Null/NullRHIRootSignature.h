// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIRootSignature.h"

class NullRHIDevice;
struct RHIRootSignatureDesc;

class NullRHIRootSignature : public RHIRootSignature
{
	friend NullRHIDevice;

protected:
	NullRHIRootSignature(std::shared_ptr<RHIDevice> device, const RHIRootSignatureDesc& desc);

public:
	virtual ~NullRHIRootSignature() noexcept override;
};