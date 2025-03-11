// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Layout/DockAttach.h"

class SLATE_API SDockPanel : public SPanel
{
	GENERATED_SLATE_BODY(SDockPanel);

public:
	struct SLATE_API NSlot : public NSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(NMargin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left)
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top)
		DECLARE_SLATE_ATTRIBUTE(EDockAttach, Dock);
	};

private:
	std::vector<NSlot> Slots;

public:
	SDockPanel();
	virtual ~SDockPanel() noexcept;

	NSlot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget& Content) const;
	void ClearSlots();
	size_t NumSlots() const { return Slots.size(); }

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
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};