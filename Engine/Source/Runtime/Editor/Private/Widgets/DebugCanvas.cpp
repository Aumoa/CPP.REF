// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/DebugCanvas.h"
#include "Widgets/Text/TextBlock.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Misc/TickScheduler.h"
#include "Diagnostics/CycleCounter.h"

GENERATE_BODY(SDebugCanvas);

SDebugCanvas::SDebugCanvas() : Super()
{
	RefreshTimer = gcnew STickScheduler();
}

void SDebugCanvas::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InDeltaTime);
	++TickCounter;
	RefreshTimer->Tick(InDeltaTime);
}

DEFINE_SLATE_CONSTRUCTOR(SDebugCanvas, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	ConstructDisplayStats();

	RefreshTimer->AddSchedule(STickScheduler::TaskInfo
	{
		.Task = [this]() { UpdateTexts(); },
		.Delay = 1.0f,
		.InitDelay = 0.0f,
		.bReliableCallCount = false
	});
}

void SDebugCanvas::UpdateTexts()
{
	static constexpr size_t Megabyte = 1024 * 1024;

	GCCounter->SetText(std::format(L"DebugCanvas:\n  Total objects: {}\n  GC Memory: {:.4f} MB\n  FPS: {}", GC.NumObjects(), GC.MemorySize() / (float)Megabyte, TickCounter));
	TickCounter = 0;

	STATGROUP_GC->SetText(CycleCounter::Get().GetNamespace(L"GC")->Trace());
	STATGROUP_Engine->SetText(CycleCounter::Get().GetNamespace(L"Engine")->Trace());
}

void SDebugCanvas::ConstructDisplayStats()
{
	AddSlot()
		.bAutoSize(true)
		[
			SNew(SVerticalBoxPanel)
			+SVerticalBoxPanel::SSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(GCCounter, STextBlock)
				.Text(L"Objects: 0")
				.Font(L"Consolas", 15.0f)
				.TintColor(NamedColors::White)
			]
			+SVerticalBoxPanel::SSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(STATGROUP_GC, STextBlock)
				.Text(L"")
				.Font(L"Consolas", 15.0f)
				.TintColor(NamedColors::White)
			]
			+SVerticalBoxPanel::SSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(STATGROUP_Engine, STextBlock)
				.Text(L"")
				.Font(L"Consolas", 15.0f)
				.TintColor(NamedColors::White)
			]
		];
}