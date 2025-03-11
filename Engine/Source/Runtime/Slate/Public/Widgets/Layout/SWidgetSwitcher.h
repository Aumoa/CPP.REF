// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SPanel.h"

class SLATE_API SWidgetSwitcher : public SPanel
{
public:
	struct SLATE_API NSlot : public NSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(NMargin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left)
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top)
	};

private:
	std::vector<NSlot> Slots;
	int32 SlotIndex = 0;

public:
	SWidgetSwitcher();
	virtual ~SWidgetSwitcher() noexcept override;

	NSlot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget& Content) const;
	void ClearSlots();
	size_t NumSlots() const { return Slots.size(); }

	void SetActiveWidgetIndex(int32 InIndex);
	int32 GetActiveWidgetIndex() const { return SlotIndex; }

	virtual size_t NumChildrens() const override { return Slots.size(); }
	virtual SWidget* GetChildrenAt(size_t InIndex) const override
	{
		if (Slots.size() > InIndex)
		{
			return Slots[InIndex].Content.get();
		}
		else
		{
			return nullptr;
		}
	}

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;
	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const override;

protected:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_SLOT_SUPPORTS(NSlot)
		DECLARE_SLATE_ATTRIBUTE(int32, SlotIndex, 0)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};