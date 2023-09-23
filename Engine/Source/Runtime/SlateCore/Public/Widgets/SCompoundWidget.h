// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Widgets/SWidget.h"
#include "Layout/HorizontalAlignment.h"
#include "Layout/VerticalAlignment.h"

class SLATECORE_API SCompoundWidget : public SWidget
{
	GENERATED_SLATE_BODY(SCompoundWidget)

public:
	struct NSlot : public NSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(NMargin, SlotPadding);
		DECLARE_SLATE_ATTRIBUTE(EHorizontalAlignment, HAlignment, EHorizontalAlignment::Left)
		DECLARE_SLATE_ATTRIBUTE(EVerticalAlignment, VAlignment, EVerticalAlignment::Top)
	};

private:
	NSlot ChildSlot;

public:
	SCompoundWidget();

	virtual void PrepassLayout() override;
	virtual void Tick(const NGeometry& AllottedGeomtry, const TimeSpan& InDeltaTime) override;

	void SetContent(std::shared_ptr<SWidget> InContent);
	std::shared_ptr<SWidget> GetContent() const { return ChildSlot.Content; }

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual void OnArrangeChildren(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry) const override;
	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;

public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_SINGLE_SLOT_SUPPORTS(NSlot)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR() {}
};