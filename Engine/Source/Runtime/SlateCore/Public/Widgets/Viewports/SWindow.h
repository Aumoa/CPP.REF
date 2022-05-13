// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SCompoundWidget.h"

namespace libty::inline SlateCore
{
	class SViewport;

	class SLATECORE_API SWindow : public SCompoundWidget
	{
		GENERATED_BODY(SWindow)

	private:
		SPROPERTY(RContext)
		SRenderContext* RContext = nullptr;

		SPROPERTY(SwapChain)
		IRHISwapChain* SwapChain = nullptr;
		int32 BufferCount;

		SPROPERTY(Viewports)
		std::vector<SViewport*> Viewports;

	public:
		SWindow();

		SFUNCTION(Inject)
		void Inject(SRenderEngine* REngine);

		void RenderWindow(SRenderContext* RContext);

		virtual size_t NumChildrens() override;
		virtual SWidget* GetChildrenAt(size_t IndexOf) override;

	protected:
		virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

	public:
		BEGIN_SLATE_ATTRIBUTE
			DECLARE_SLATE_ATTRIBUTE(int32, BufferCount, 3)
		END_SLATE_ATTRIBUTE;

		DECLARE_SLATE_CONSTRUCTOR();
	};
}