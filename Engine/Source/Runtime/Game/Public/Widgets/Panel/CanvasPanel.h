// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "SlateMinimal.h"
#include "Panel.h"
#include "Widgets/SlotBase.h"
#include "Widgets/DeclarativeSyntaxSupports.h"

class GAME_API SCanvasPanel : public SPanel
{
	GENERATED_BODY(SCanvasPanel)

public:
	class Slot : public TSlotBase<Slot>
	{
	public:
		Slot();

		DECLARE_SLATE_ATTRIBUTE(Margin, Offset);
		DECLARE_SLATE_ATTRIBUTE(Anchors, Anchors);
		DECLARE_SLATE_ATTRIBUTE(Vector2, Alignment);
		DECLARE_SLATE_ATTRIBUTE(float, ZOrder, = 0);
		DECLARE_SLATE_ATTRIBUTE(bool, bAutoSize);
	};

private:
	struct ChildZOrder
	{
		int32 ChildIndex;
		float ZOrder;
	};

private:
	std::vector<Slot*> _childrens;

public:
	SCanvasPanel(const std::wstring& name = GenerateAutoNumberedName());
	virtual ~SCanvasPanel() override;

protected:
	virtual int32 OnPaint(PaintArgs* paintArgs, const Geometry& allottedGeometry, const Rect& cullingRect, SlateWindowElementList* drawElements, int32 layer, bool bParentEnabled) const override;
	virtual void OnArrangeChildren(ArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const override;

public:
	BEGIN_SLATE_ATTRIBUTE
		DECLARE_SLATE_SLOT_SUPPORTS(Slot)
	END_SLATE_ATTRIBUTE

	DECLARE_SLATE_CONSTRUCTOR();

private:
	static std::wstring GenerateAutoNumberedName();
};