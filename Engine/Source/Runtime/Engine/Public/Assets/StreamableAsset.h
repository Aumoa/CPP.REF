// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class ENGINE_API StreamableAsset
{
protected:
	StreamableAsset();

public:
	virtual Task<> StreamIn() = 0;
	virtual void StreamOut() = 0;

	virtual bool IsStreamIn() const = 0;
};