// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Application/Viewport.h"
#include "Draw/SlateDrawCollector.h"
#include "Widgets/Image/Image.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "SceneRendering/RenderTargets/RaytraceSceneRenderTarget.h"
#include "GameEngine.h"

GENERATE_BODY(SViewport);
DEFINE_LOG_CATEGORY(LogViewport);

SViewport::SViewport() : Super()
{
}

SViewport::~SViewport()
{
	Widgets.clear();
}

void SViewport::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	Super::Tick(AllottedGeometry, InDeltaTime);

	Vector2N LocalSize = Vector<>::Cast<Vector2N>(AllottedGeometry.GetLocalSize());
	SetRenderSize(LocalSize);
}

bool SViewport::PrepassLayout()
{
	bool bShouldBePrepass = false;

	if (ShouldBePrepassLayout())
	{
		ReallocRenderTarget();
		bShouldBePrepass = true;
	}

	return Super::PrepassLayout() || bShouldBePrepass;
}

void SViewport::SetRenderSize(Vector2N InRenderSize)
{
	if (RenderSize != InRenderSize)
	{
		RenderSize = InRenderSize;
		InvalidateLayoutAndVolatility();
	}
}

Vector2N SViewport::GetRenderSize()
{
	return RenderSize;
}

void SViewport::PopulateCommandLists(IRHIDeviceContext* InDeviceContext)
{
}

void SViewport::AddToViewport(SWidget* InWidget)
{
	Widgets.emplace_back(InWidget);
}

void SViewport::RemoveFromViewport(SWidget* InWidget)
{
	for (auto It = Widgets.begin(); It != Widgets.end(); ++It)
	{
		if (*It == InWidget)
		{
			Widgets.erase(It);
			break;
		}
	}
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, InAttr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(InAttr);
	RenderSize = InAttr._RenderSize;
	RenderTarget = gcnew SRaytraceSceneRenderTarget();
	ReallocRenderTarget();
}

Vector2 SViewport::ComputeDesiredSize()
{
	return Cast<Vector2>(RenderSize);
}

void SViewport::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	for (auto& Widget : Widgets)
	{
		ESlateVisibility Visibility = Widget->GetVisibility();
		if (ArrangedChildrens.Accepts(Visibility))
		{
			ArrangedChildrens.AddWidget(Visibility, AllottedGeometry.MakeChild(
				Widget,
				Vector2(0.0f, 0.0f),
				AllottedGeometry.GetLocalSize()
			));
		}
	}

	if (SceneImage)
	{
		ArrangedChildrens.AddWidget(ESlateVisibility::Visible, AllottedGeometry.MakeChild(
			SceneImage,
			Vector2::Zero(),
			AllottedGeometry.GetLocalSize()
		));
	}
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return Super::OnPaint(Args, AllottedGeometry, CullingRect, DrawCollector, InLayer + 1, bParentEnabled);
}

size_t SViewport::NumChildrens()
{
	return Widgets.size();
}

SWidget* SViewport::GetChildrenAt(size_t IndexOf)
{
	return Widgets[IndexOf];
}

void SViewport::ReallocRenderTarget()
{
	auto RenderSystem = GEngine->GetEngineSubsystem<SGameRenderSystem>();
	if (RenderSystem)
	{
		if (ensure(RenderSize.X != 0 && RenderSize.Y != 0))
		{
			SE_LOG(LogViewport, Verbose, L"Viewport render targets are reallocated to [{}x{}].", RenderSize.X, RenderSize.Y);
		}
	}
}