// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "CompoundWidget.h"
#include "RHI/RHIEnums.h"

DECLARE_LOG_CATEGORY(GAME_API, LogViewport);

interface IRHITexture2D;

class GAME_API SViewport : public SCompoundWidget
{
	GENERATED_BODY(SViewport)

private:
	Vector2N RenderSize;
	ERHIPixelFormat RenderTargetFormat;

	IRHITexture2D* RenderTarget = nullptr;
	IRHITexture2D* DepthStencil = nullptr;

public:
	SViewport();
	~SViewport();

	void SetRenderSize(const Vector2N& InRenderSize);
	Vector2N GetRenderSize();

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Vector2N, RenderSize)
		DECLARE_SLATE_ATTRIBUTE(ERHIPixelFormat, RenderTargetFormat, ERHIPixelFormat::B8G8R8A8_UNORM)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* ArrangedChildrens, const Geometry& AllottedGeometry) override;

private:
	void ReallocRenderTarget();
	void DestroyRenderTarget_GameThread();
};