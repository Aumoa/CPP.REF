// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class NRHIViewport;

class RENDERCORE_API NSceneRenderer
{
private:
	std::shared_ptr<NRHIViewport> Viewport;

protected:
	NSceneRenderer();

public:
	virtual ~NSceneRenderer() noexcept;

	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;

	virtual void SetViewport(std::shared_ptr<NRHIViewport> InViewport);
	std::shared_ptr<NRHIViewport> GetViewport() const;
};