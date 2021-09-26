// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PanelWidget.h"
#include "Widgets/SlotBase.h"
#include "Layout/Anchors.h"

class GAME_API SCanvasPanel : public SPanelWidget
{
	GENERATED_BODY(SCanvasPanel)

public:
	class Slot : public TSlotBase<Slot>
	{
	public:
		Slot()
		{
		}

		DECLARE_SLATE_ATTRIBUTE(Margin, Offset, = Margin(0, 0, 100.0f, 100.0f));
		DECLARE_SLATE_ATTRIBUTE(::Anchors, Anchors);
		DECLARE_SLATE_ATTRIBUTE(Vector2, Alignment);
		DECLARE_SLATE_ATTRIBUTE(float, ZOrder, = 0);
		DECLARE_SLATE_ATTRIBUTE(bool, bAutoSize, = false);
	};

private:
	struct ChildZOrder
	{
		int32 ChildIndex;
		float ZOrder;
	};

private:
	std::vector<Slot> _slots;

public:
	SCanvasPanel(const std::wstring& name);
	virtual ~SCanvasPanel() override;

	virtual Vector2 GetDesiredSize() const override;

	Slot& AddSlot();
	bool RemoveSlot(size_t index);
	size_t FindSlot(const SWidget* content) const;
	void ClearSlots();
	size_t NumSlots() const;

protected:
	virtual int32 OnPaint(SPaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SSlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_SLOT_SUPPORTS(Slot)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};