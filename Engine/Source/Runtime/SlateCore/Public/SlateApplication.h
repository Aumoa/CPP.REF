// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline SlateCore
{
	class SWindow;

	class SLATECORE_API SSlateApplication : implements(SObject)
	{
		GENERATED_BODY(SSlateApplication, AutoRegistration());

	private:
		SPROPERTY(RContext)
		SRenderContext* RContext = nullptr;
		SPROPERTY(CoreWindow)
		SWindow* CoreWindow = nullptr;

	public:
		SSlateApplication();

		SFUNCTION(Inject)
		void Inject(SRenderEngine* REngine);

		void TickAndPaint(float InDeltaTime);
		void RenderApp(SRenderContext* RContext);
	};
}