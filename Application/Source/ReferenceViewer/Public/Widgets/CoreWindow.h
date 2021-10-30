// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Window.h"

class SCanvasPanel;
class SImage;

class REFERENCEVIEWER_API SCoreWindow : public SWindow
{
	GENERATED_BODY(SCoreWindow)

public:
	SCoreWindow();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime) override;

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

public:
	DECLARE_SLATE_CONSTRUCTOR();

private:
	SCanvasPanel* CanvasPanel = nullptr;
	SImage* RedBox = nullptr;
	SImage* GreenBox = nullptr;
	SImage* BlueBox = nullptr;
};