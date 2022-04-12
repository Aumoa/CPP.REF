// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "SPanelWidget.h"

namespace libty::inline SlateCore
{
	class SLATECORE_API SCanvasPanel : extends(SPanelWidget)
	{
		GENERATED_BODY(SCanvasPanel);

	public:
		class SLATECORE_API SSlot : public TSlotBase<SSlot>
		{
			GENERATED_BODY(SSlot)

		public:
			SSlot() : Super()
			{
			}

			DECLARE_SLATE_ATTRIBUTE(Margin, Offset, Margin(0, 0, 100.0f, 100.0f));
			DECLARE_SLATE_ATTRIBUTE(::libty::Anchors, Anchors);
			DECLARE_SLATE_ATTRIBUTE(Vector2, Alignment);
			DECLARE_SLATE_ATTRIBUTE(float, ZOrder, 0);
			DECLARE_SLATE_ATTRIBUTE(bool, bAutoSize, false);
		};

	private:
		struct ChildZOrder
		{
			int32 ChildIndex;
			float ZOrder;
		};

	private:
		SPROPERTY(Slots)
		std::vector<SSlot*> Slots;

	public:
		SCanvasPanel();
		virtual ~SCanvasPanel() override;

		SSlot& AddSlot();
		bool RemoveSlot(size_t Index);
		size_t FindSlot(const SWidget* Content);
		void ClearSlots();
		size_t NumSlots();

		virtual size_t NumChildrens() override;
		virtual SWidget* GetChildrenAt(size_t IndexOf) override;

	protected:
		virtual Vector2 ComputeDesiredSize() override;
		virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SSlateDrawCollector* DrawCollector, int32 InLayer, bool bParentEnabled) override;
		virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;

		void ArrangeLayeredChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, std::vector<bool>& ChildLayers);

	public:
		BEGIN_SLATE_ATTRIBUTE
			DECLARE_SLATE_SLOT_SUPPORTS(SSlot)
		END_SLATE_ATTRIBUTE

		DECLARE_SLATE_CONSTRUCTOR();
	};
}