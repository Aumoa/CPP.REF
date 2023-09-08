// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWindow;

class SLATECORE_API NSlateRenderer
{
protected:
	NSlateRenderer();

public:
	virtual ~NSlateRenderer() noexcept;

	virtual void BeginFrame() {}
	virtual void EndFrame() {}
	virtual void FlushCommands() {}

	virtual void CreateViewport(SWindow& InWindow) = 0;
};