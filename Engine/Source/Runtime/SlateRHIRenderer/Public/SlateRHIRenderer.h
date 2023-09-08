// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Rendering/SlateRenderer.h"

class SLATERHIRENDERER_API NSlateRHIRenderer : public NSlateRenderer
{
public:
	NSlateRHIRenderer();

	virtual void BeginFrame() override;
	virtual void EndFrame() override;
	virtual void FlushCommands() override;

	virtual void CreateViewport(SWindow& InWindow) override;
};