// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SWidget.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SCompoundWidget : public SWidget
	{
		GENERATED_BODY(SCompoundWidget);

	public:
		SCompoundWidget();

		virtual void Tick(const Geometry& AllottedGeometry, float InDeltaTime);
		virtual bool PrepassLayout() override;

		virtual bool SendMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) override;
		virtual bool SendMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;
		virtual bool SendMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;
		virtual bool SendKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) override;
		virtual bool SendIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs) override;

		virtual size_t NumChildrens() = 0;
		virtual SWidget* GetChildrenAt(size_t IndexOf) = 0;

	protected:
		virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;

		int32 PaintArrangedChildrens(const PaintArgs& Args, ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled);

		virtual bool OnReceiveMouseMoved(const Geometry& AllottedGeometry, const Vector2N& Location) override;
		virtual bool OnReceiveMouseWheelScrolled(const Geometry& AllottedGeometry, int32 ScrollDelta) override;
		virtual bool OnReceiveMouseEvent(const Geometry& AllottedGeometry, const Vector2N& Location, EMouseButton Button, EMouseButtonEvent Event) override;
		virtual bool OnReceiveKeyboardEvent(const Geometry& AllottedGeometry, EKey Key, EKeyboardEvent Event) override;
		virtual bool OnReceiveIMEEvent(const Geometry& AllottedGeometry, const IMEEvent& EventArgs) override;
	};
}