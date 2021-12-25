// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/CanvasPanel.h"

class STickScheduler;
class STextBlock;

class GAME_API SDebugCanvas : public SCanvasPanel
{
	GENERATED_BODY(SDebugCanvas)

public:
	SDebugCanvas();

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime) override;

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	SPROPERTY(RefreshTimer)
	STickScheduler* RefreshTimer = nullptr;
	SPROPERTY(GCCounter)
	STextBlock* GCCounter = nullptr;
	SPROPERTY(GCTracer)
	STextBlock* GCTracer = nullptr;

private:
	void UpdateTexts();
};