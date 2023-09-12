// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Layout/HorizontalAlignment.h"
#include "Layout/VerticalAlignment.h"
#include "Layout/SizeParam.h"
#include "Layout/Margin.h"
#include "Layout/Orientation.h"
#include "Layout/FlowDirection.h"

class SLATE_API SBoxPanel : public SPanel
{
	GENERATED_SLATE_BODY(SBoxPanel)

public:
	struct SLATE_API NSlot : public NSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left);
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top);
		DECLARE_SLATE_ATTRIBUTE(NSizeParam, SizeParam);
		DECLARE_SLATE_ATTRIBUTE(NMargin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(float, MaxSize, 0);
	};

private:
	EOrientation Orientation;
	std::vector<NSlot> Slots;

protected:
	SBoxPanel(EOrientation InOrientation);

	void SetOrientation(EOrientation InOrientation);

public:
	virtual ~SBoxPanel() noexcept override;

	NSlot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget& InContent) const;
	void ClearSlots();
	size_t NumSlots() const { return Slots.size(); }

	EOrientation GetOrientation() const { return Orientation; }

	virtual size_t NumChildrens() const override { return NumSlots(); }
	virtual SWidget* GetChildrenAt(size_t InIndex) const override;

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual void OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const override;

private:
	void ArrangeChildrenAlong(EOrientation InOrientation, EFlowDirection InLayoutFlow, const NGeometry& AllottedGeometry, NArrangedChildrens& ArrangedChildrens) const;
	static Vector2 ComputeDesiredSizeForBox(EOrientation InOrientation, const std::vector<NSlot>& Slots);

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_SLOT_SUPPORTS(NSlot)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};