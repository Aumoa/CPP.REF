// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SLeafWidget.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SViewport : public SLeafWidget
	{
		GENERATED_BODY(SViewport);

	private:
		Vector2N SurfaceSize;

		SPROPERTY(RContext)
		SRenderContext* RContext = nullptr;
		SPROPERTY(Surface)
		IRHIResource* Surface = nullptr;
		SPROPERTY(RThread);
		SRenderThread* RThread = nullptr;

	public:
		SViewport();

		SFUNCTION(Inject)
		void Inject(SRenderEngine* REngine, SRenderThread* RThread);

		virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime) override;
		virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

	private:
		void ReallocateSurface(const Vector2& SurfaceVector);
	};
}