// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Viewports/SViewport.h"

using namespace ::libty;

SViewport::SViewport()
	: Super()
{
}

void SViewport::Inject(SRenderEngine* REngine)
{
	RContext = REngine->CreateRenderContext();
}

void SViewport::Tick(const Geometry& AllottedGeometry, float InDeltaTime)
{
	ReallocateSurface(AllottedGeometry.GetLocalSize());

	Super::Tick(AllottedGeometry, InDeltaTime);
}

int32 SViewport::OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled)
{
	return InLayer;
}

void SViewport::ReallocateSurface(const Vector2& SurfaceVector)
{
	Vector2N Size = Vector<>::Cast<int32>(SurfaceVector);
	if (SurfaceSize != Size)
	{
		RHIHeapProperties HeapProp = { .Type = ERHIHeapType::Default };
		RHIResourceDesc Resource =
		{
			.Dimension = ERHIResourceDimension::Texture2D,
			.Width = (uint64)Size.X,
			.Height = (uint32)Size.Y,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = ERHIPixelFormat::B8G8R8A8_UNORM,
			.SampleDesc = { 1, 0 },
			.Flags = ERHIResourceFlags::AllowRenderTarget
		};

		constexpr bool b = std::same_as<decltype(ERHIResourceStates::PixelShaderResource)::__Tag__, ERHIResourceStates::__Tag__>;

		Surface = RContext->OwningDevice->CreateCommittedResource(HeapProp, ERHIHeapFlags::None, Resource, ERHIResourceStates::PixelShaderResource | ERHIResourceStates::CopySource, nullptr);
		SurfaceSize = Size;
	}
}