// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Panel/CanvasPanel.h"
#include "Widgets/Text/TextBlock.h"

class STickScheduler;
class STextBlock;
class SVerticalBoxPanel;
class CycleCounterNamespace;

class EDITOR_API SDebugCanvas : public SCanvasPanel
{
	GENERATED_BODY(SDebugCanvas)

private:
	static SDebugCanvas* sInstance;

public:
	SDebugCanvas();
	virtual ~SDebugCanvas() noexcept override;

	virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime) override;

	void ToggleSTAT(std::wstring_view Stat);

	static SDebugCanvas* Get();

public:
	BEGIN_SLATE_ATTRIBUTE
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	uint64 TickCounter = 0;

	SPROPERTY(RefreshTimer)
	STickScheduler* RefreshTimer = nullptr;
	SPROPERTY(GCCounter)
	STextBlock* GCCounter = nullptr;

	struct StatGroupView
	{
		GENERATED_STRUCT_BODY(StatGroupView)

		SPROPERTY(TextBlock)
		STextBlock* TextBlock = nullptr;
		CycleCounterNamespace* Namespace = nullptr;
	};

	SPROPERTY(VBox)
	SVerticalBoxPanel* VBox = nullptr;
	SPROPERTY(STATGROUPS)
	std::map<std::wstring, StatGroupView, std::less<>> STATGROUPS;

private:
	void UpdateTexts();
	void ConstructDisplayStats();
};