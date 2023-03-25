// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline SlateCore
{
	class SLATECORE_API SSlateRenderer : implements(SObject)
	{
		GENERATED_BODY(SSlateRenderer);

	public:
		SSlateRenderer(IRHIDevice* device);

		virtual void BeginFrame(SRenderContext* renderContext);
		//virtual void RenderViewport(SViewport* viewport);
		virtual void EndFrame();
	};
}