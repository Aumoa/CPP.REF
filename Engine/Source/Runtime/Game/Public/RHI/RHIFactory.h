// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIFactoryLevel.h"

class RHIDevice;

class GAME_API RHIFactory : public std::enable_shared_from_this<RHIFactory>
{
private:
	bool _bEnableDebugLayer = false;

protected:
	RHIFactory(bool bEnableDeubgLayer = false);

public:
	virtual ~RHIFactory() noexcept;

	bool IsDebugLayerEnabled() const;

	virtual std::shared_ptr<RHIDevice> CreateDevice() = 0;

public:
	static std::shared_ptr<RHIFactory> CreateFactory();
};