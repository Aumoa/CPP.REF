// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SLATECORE_API NSlateRenderer
{
protected:
	NSlateRenderer();

public:
	virtual ~NSlateRenderer() noexcept;

	virtual void BeginFrame() {}
	virtual void EndFrame() {}
	virtual void FlushCommands() {}
};