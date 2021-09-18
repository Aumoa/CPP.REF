// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "PanelWidget.h"
#include "Widgets/SlotBase.h"

class SLATECORE_API SBoxPanel : public SPanelWidget
{
	GENERATED_BODY(SBoxPanel)

public:
	class Slot : public TSlotBase<Slot>
	{
	public:
		Slot()
		{
		}

		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, = EHorizontalAlignment::Left);
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, = EVerticalAlignment::Top);
		DECLARE_SLATE_ATTRIBUTE(::SizeParam, SizeParam);
		DECLARE_SLATE_ATTRIBUTE(Margin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(float, MaxSize, = 0);
	};

private:
	EOrientation _orientation;
	std::vector<Slot> _slots;

protected:
	SBoxPanel(const std::wstring& name, EOrientation orientation);
	virtual ~SBoxPanel() override;

	virtual Vector2 GetDesiredSize() const override;

public:
	Slot& AddSlot();
	bool RemoveSlot(size_t index);
	size_t FindSlot(const SWidget* content) const;
	void ClearSlots();
	size_t NumSlots() const;

	inline EOrientation GetOrientation() const { return _orientation; }

protected:
	virtual void OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;

private:
	void ArrangeChildrenAlong(EOrientation orientation, EFlowDirection layoutFlow, const Geometry& allottedGeometry, SArrangedChildrens* arrangedChildrens) const;
	static Vector2 ComputeDesiredSizeForBox(EOrientation orientation, const std::vector<Slot>& slots);

	static constexpr Margin LayoutPaddingWithFlow(const Margin& padding, EFlowDirection layoutFlow)
	{
		if (layoutFlow == EFlowDirection::RightToLeft)
		{
			return Margin(padding.Right, padding.Top, padding.Left, padding.Bottom);
		}
		else
		{
			return padding;
		}
	}

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_SLOT_SUPPORTS(Slot)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();
};