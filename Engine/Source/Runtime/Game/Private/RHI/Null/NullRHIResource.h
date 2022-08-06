// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIResource.h"

class NullRHIDevice;

class NullRHIResource : public RHIResource
{
	friend NullRHIDevice;

private:
	NullRHIResource(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHIResource() noexcept override;
};