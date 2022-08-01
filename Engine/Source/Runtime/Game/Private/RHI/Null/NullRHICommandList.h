// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandList.h"

class NullRHIDevice;

class NullRHICommandList : public RHICommandList
{
	friend NullRHIDevice;

private:
	bool _begun = false;

private:
	NullRHICommandList(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHICommandList() noexcept override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
};