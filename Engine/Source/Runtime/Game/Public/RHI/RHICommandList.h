// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class GAME_API RHICommandList : public RHIDeviceResource
{
protected:
	RHICommandList(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHICommandList() noexcept override;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
};