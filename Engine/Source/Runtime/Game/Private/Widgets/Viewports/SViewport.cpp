// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SViewport.h"
#include "Widgets/Viewports/SWindow.h"
#include "Widgets/Drawing/PaintArgs.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHIStructures.h"
#include "RHI/RHIDescriptorTable.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/SceneRenderContext.h"
#include "SViewport.gen.cpp"

constexpr LogCategory LogViewport = LogCategory(TEXT("LogViewport"));

SViewport::SViewport()
	: Super()
{
}

void SViewport::Tick(const Geometry& AllottedGeometry, const TimeSpan& DeltaTime)
{
	Super::Tick(AllottedGeometry, DeltaTime);
	TryResizeViewport(AllottedGeometry);
	ComposeRaytracingShaders();
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return InLayer;
}

std::shared_ptr<RHIResource> SViewport::PresentViewport(SceneRenderContext& Context, RHICommandList* CmdList)
{
	RHIResourceBarrier Barrier =
	{
		.Type = ERHIResourceBarrierType::Transition,
		.Flags = ERHIResourceBarrierFlags::None,
		.Transition =
		{
			.pResource = RenderTarget.ColorTarget.get(),
			.Subresource = 0,
			.StateBefore = ERHIResourceStates::CopySource,
			.StateAfter = ERHIResourceStates::UnorderedAccess
		}
	};

	CmdList->ResourceBarrier({ &Barrier, 1 });

	std::vector Heaps = { DescriptorTable.get() };
	CmdList->SetDescriptorTables(Heaps);

	for (auto& Scene : Context.SceneViews)
	{
		Context.Renderer->Render(Scene, CmdList);
	}

	std::swap(Barrier.Transition.StateBefore, Barrier.Transition.StateAfter);
	CmdList->ResourceBarrier({ &Barrier, 1 });

	return RenderTarget.ColorTarget;
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	Device = Attr._Window->GetDevice();
}

void SViewport::TryResizeViewport(const Geometry& AllottedGeometry)
{
	Vector2N Size = Vector<>::Cast<int32>(AllottedGeometry.GetLocalSize());
	if (RenderTarget.Size != Size)
	{
		EnqueueRenderThreadWork([ref = std::move(RenderTarget), Size]() mutable
			{
				Log::Info(LogViewport, TEXT("Viewport resized from [{0}] to [{1}]."), ref.Size, Size);
			});

		RenderTarget = RHIRaytracingRenderTarget::Create(Device, Size);
	}
}

void SViewport::ComposeRaytracingShaders()
{	
	if (!DescriptorTable)
	{
		DescriptorTable = Device->CreateDescriptorTable(1);
	}
}