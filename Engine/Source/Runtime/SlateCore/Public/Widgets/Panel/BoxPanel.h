// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "PanelWidget.h"

class SLATECORE_API SBoxPanel : public SPanelWidget
{
	GENERATED_BODY(SBoxPanel)

public:
	class SLATECORE_API SSlot : public TSlotBase<SSlot>
	{
		GENERATED_BODY(SSlot)

	public:
		SSlot() : Super()
		{
		}

		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left);
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top);
		DECLARE_SLATE_ATTRIBUTE(::SizeParam, SizeParam);
		DECLARE_SLATE_ATTRIBUTE(Margin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(float, MaxSize, 0);
	};

private:
	EOrientation Orientation;
	SPROPERTY(Slots)
	std::vector<SSlot*> Slots;

protected:
	SBoxPanel(EOrientation Orientation = EOrientation::Vertical);

	void SetOrientation(EOrientation InOrientation);

public:
	SSlot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget* Content);
	void ClearSlots();
	size_t NumSlots();

	EOrientation GetOrientation();

	virtual size_t NumChildrens() override;
	virtual SWidget* GetChildrenAt(size_t IndexOf) override;

protected:
	virtual Vector2 ComputeDesiredSize() override;
	virtual void OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry) override;

private:
	void ArrangeChildrenAlong(EOrientation InOrientation, EFlowDirection InLayoutFlow, const Geometry& AllottedGeometry, ArrangedChildrens& ArrangedChildrens);
	static Vector2 ComputeDesiredSizeForBox(EOrientation InOrientation, const std::vector<SSlot*>& Slots);

	static constexpr Margin LayoutPaddingWithFlow(const Margin& Padding, EFlowDirection LayoutFlow)
	{
		if (LayoutFlow == EFlowDirection::RightToLeft)
		{
			return Margin(Padding.Right, Padding.Top, Padding.Left, Padding.Bottom);
		}
		else
		{
			return Padding;
		}
	}

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_SLOT_SUPPORTS(SSlot)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};