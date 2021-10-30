// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PanelWidget.h"
#include "Widgets/SlotBase.h"
#include "Layout/Anchors.h"

class APPENGINE_API SCanvasPanel : public SPanelWidget
{
	GENERATED_BODY(SCanvasPanel)

public:
	class Slot : public TSlotBase<Slot>
	{
	public:
		Slot()
		{
		}

		DECLARE_SLATE_ATTRIBUTE(Margin, Offset, Margin(0, 0, 100.0f, 100.0f));
		DECLARE_SLATE_ATTRIBUTE(::Anchors, Anchors);
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
	std::vector<Slot> Slots;

public:
	SCanvasPanel();
	virtual ~SCanvasPanel() override;

	virtual Vector2 GetDesiredSize() override;

	Slot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget* Content);
	void ClearSlots();
	size_t NumSlots();

protected:
	virtual int32 OnPaint(const PaintArgs& Args, const Geometry& AllottedGeometry, const Rect& CullingRect, SlateWindowElementList& InDrawElements, int32 InLayer, bool bParentEnabled) override;
	virtual void OnArrangeChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry) override;
	
	void ArrangeLayeredChildren(ArrangedChildrens& InoutArrangedChildrens, const Geometry& AllottedGeometry, std::vector<bool>& ChildLayers);

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_SLOT_SUPPORTS(Slot)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};