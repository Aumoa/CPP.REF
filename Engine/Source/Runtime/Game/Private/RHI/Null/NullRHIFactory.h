// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIFactory.h"

class NullRHIFactory : public RHIFactory
{
	friend RHIFactory;

private:
	NullRHIFactory();

public:
	virtual ~NullRHIFactory() noexcept override;

	virtual std::shared_ptr<RHIDevice> CreateDevice() override;
};