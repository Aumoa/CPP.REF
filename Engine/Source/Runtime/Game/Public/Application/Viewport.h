// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CompoundWidget.h"
#include "RHI/RHIEnums.h"

DECLARE_LOG_CATEGORY(GAME_API, LogViewport);

struct IRHIDeviceContext;
class SWorld;
class SImage;
class SRaytraceSceneRenderTarget;

class GAME_API SViewport : public SCompoundWidget
{
	GENERATED_BODY(SViewport)

private:
	Vector2N RenderSize;

	SPROPERTY(SceneImage)
	SImage* SceneImage = nullptr;

private:
	SPROPERTY(RenderTarget)
	SRaytraceSceneRenderTarget* RenderTarget = nullptr;

public:
	SViewport();
	~SViewport();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime) override;
	virtual bool PrepassLayout() override;

	void SetRenderSize(Vector2N InRenderSize);
	Vector2N GetRenderSize();

	void PopulateCommandLists(IRHIDeviceContext* InDeviceContext);

private:
	SPROPERTY(Widgets)
	std::vector<SWidget*> Widgets;

public:
	void AddToViewport(SWidget* InWidget);
	void RemoveFromViewport(SWidget* InWidget);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_ATTRIBUTE(Vector2N, RenderSize)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() override;
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

	virtual size_t NumChildrens() override;
	virtual SWidget* GetChildrenAt(size_t IndexOf) override;

private:
	void ReallocRenderTarget();
};