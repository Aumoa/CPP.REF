// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/DebugCanvas.h"
#include "Widgets/Text/TextBlock.h"
#include "Widgets/Panel/VerticalBoxPanel.h"
#include "Misc/TickScheduler.h"
#include "Diagnostics/CycleCounter.h"
#include <boost/algorithm/string.hpp>

GENERATE_BODY(SDebugCanvas);
DEFINE_LOG_CATEGORY(LogDebugCanvas);

SDebugCanvas* SDebugCanvas::sInstance;

SDebugCanvas::SDebugCanvas() : Super()
{
	checkf(sInstance == nullptr, L"Duplicated singleton instance.");
	sInstance = this;
	RefreshTimer = gcnew STickScheduler();
}

SDebugCanvas::~SDebugCanvas()
{
	sInstance = nullptr;
}

void SDebugCanvas::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InDeltaTime);
	++TickCounter;
	RefreshTimer->Tick(InDeltaTime);
}

void SDebugCanvas::ToggleSTAT(std::wstring_view Stat)
{
	auto It = STATGROUPS.find(Stat);
	if (It == STATGROUPS.end())
	{
		StatGroupView View;
		View.Namespace = CycleCounter::Get().GetNamespace(Stat);
		if (View.Namespace == nullptr)
		{
			SE_LOG(LogDebugCanvas, Warning, L"Couldn't find STATGROUP({}).", Stat);
			return;
		}

		VBox->AddSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(View.TextBlock, STextBlock)
				.Font(L"Consolas", 15.0f)
				.TintColor(NamedColors::White)
				.Text(View.Namespace->Trace())
			];

		STATGROUPS.emplace(std::wstring(Stat), View);
	}
	else
	{
		StatGroupView& View = It->second;
		size_t IndexOf = VBox->FindSlot(View.TextBlock);
		if (IndexOf == -1)
		{
			return;
		}

		VBox->RemoveSlot(IndexOf);
		STATGROUPS.erase(It);
	}
}

SDebugCanvas* SDebugCanvas::Get()
{
	return sInstance;
}

DEFINE_SLATE_CONSTRUCTOR(SDebugCanvas, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	ConstructDisplayStats();
	RefreshTimer->AddSchedule(
	{
		.Task = std::bind(&SDebugCanvas::UpdateTexts, this),
		.Delay = 1.0f,
		.InitDelay = 1.0f
	});
}

void SDebugCanvas::UpdateTexts()
{
	static constexpr size_t Megabyte = 1024 * 1024;

	GCCounter->SetText(std::format(L"DebugCanvas:\n  Total objects: {}\n  Used Memory: {:.4f} MB\n  FPS: {}", GC.NumObjects(), GC.MemorySize() / (float)Megabyte, TickCounter));
	TickCounter = 0;

	for (auto& [Key, View] : STATGROUPS)
	{
		View.TextBlock->SetText(View.Namespace->Trace());
	}
}

void SDebugCanvas::ConstructDisplayStats()
{
	AddSlot()
		.bAutoSize(true)
		[
			SAssignNew(VBox, SVerticalBoxPanel)
			+SVerticalBoxPanel::SSlot()
			.SizeParam(ESizeRule::Auto, 1.0f)
			[
				SAssignNew(GCCounter, STextBlock)
				.Text(L"Objects: 0")
				.Font(L"Consolas", 15.0f)
				.TintColor(NamedColors::White)
			]
		];
}