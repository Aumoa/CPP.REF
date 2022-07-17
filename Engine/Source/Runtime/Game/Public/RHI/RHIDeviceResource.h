// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHIDevice;

class GAME_API RHIDeviceResource : public std::enable_shared_from_this<RHIDeviceResource>
{
private:
	std::shared_ptr<RHIDevice> _device;

protected:
	RHIDeviceResource(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHIDeviceResource() noexcept;

	std::shared_ptr<RHIDevice> GetDevice() const;
};