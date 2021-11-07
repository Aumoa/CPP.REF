// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"
#include "RHI/RHIEnums.h"

DECLARE_LOG_CATEGORY(GAME_API, LogViewport);

interface IRHITexture2D;
interface IRHIDeviceContext;
interface IRHIRenderTargetView;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;
class SWorld;

class GAME_API SViewport : public SCompoundWidget
{
	GENERATED_BODY(SViewport)

private:
	Vector2N RenderSize;
	ERHIPixelFormat RenderTargetFormat = ERHIPixelFormat::Unknown;

	IRHIRenderTargetView* RTV = nullptr;
	IRHIDepthStencilView* DSV = nullptr;
	IRHIShaderResourceView* SRV = nullptr;
	IRHITexture2D* RenderTarget = nullptr;
	IRHITexture2D* DepthStencil = nullptr;
	IRHIDeviceContext* DeviceContext = nullptr;

	SWorld* GameWorld = nullptr;

public:
	SViewport();
	~SViewport();

	void SetRenderSize(Vector2N InRenderSize);
	Vector2N GetRenderSize();

	void SetGameWorld(SWorld* InGameWorld);
	void PopulateCommandLists(IRHIDeviceContext* InDeviceContext);

	virtual Vector2 GetDesiredSize() override;

private:
	std::vector<std::shared_ptr<SWidget>> Widgets;

public:
	void AddToViewport(SWidget* InWidget);
	void RemoveFromViewport(SWidget* InWidget);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Vector2N, RenderSize)
		DECLARE_SLATE_ATTRIBUTE(ERHIPixelFormat, RenderTargetFormat, ERHIPixelFormat::B8G8R8A8_UNORM)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;

private:
	void ReallocRenderTarget();
	void DestroyRenderTarget_GameThread();
};