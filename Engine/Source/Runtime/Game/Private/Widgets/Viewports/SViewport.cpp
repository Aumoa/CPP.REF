// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SViewport.h"
#include "Widgets/Viewports/SWindow.h"
#include "RHI/RHIRenderThread.h"

constexpr LogCategory LogViewport = LogCategory(TEXT("LogViewport"));

SViewport::SViewport()
	: Super()
{
}

void SViewport::Tick(const Geometry& AllottedGeometry, const TimeSpan& deltaTime)
{
	Super::Tick(AllottedGeometry, deltaTime);
	TryResizeViewport(AllottedGeometry);
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return InLayer;
}

DEFINE_SLATE_CONSTRUCTOR(SViewport, attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(attr);

	_device = attr._Window->GetDevice();
}

void SViewport::TryResizeViewport(const Geometry& allottedGeometry)
{
	Vector2N size = Vector<>::Cast<int32>(allottedGeometry.GetLocalSize());
	if (_renderTarget.Size != size)
	{
		EnqueueRenderThreadWork([ref = std::move(_renderTarget), size]() mutable
		{
			Log::Info(LogViewport, TEXT("Viewport resized from [{0}] to [{1}]."), ref.Size, size);
		});

		_renderTarget = RHIRaytracingRenderTarget::Create(_device, size);
	}
}