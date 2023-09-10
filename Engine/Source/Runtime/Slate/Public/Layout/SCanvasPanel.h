// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Layout/Anchors.h"
#include "Layout/Margin.h"

class SLATE_API SCanvasPanel : public SPanel
{
public:
	struct SLATE_API NSlot : public NPanelSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(NMargin, Offset, NMargin(0, 0, 100.0f, 100.0f));
		DECLARE_SLATE_ATTRIBUTE(NAnchors, Anchors);
		DECLARE_SLATE_ATTRIBUTE(Vector2, Alignment);
		DECLARE_SLATE_ATTRIBUTE(int32, ZOrder, 0);
		DECLARE_SLATE_ATTRIBUTE(bool, bAutoSize, false);

		inline const NMargin& GetOffset() const { return _Offset; }
		inline const NAnchors& GetAnchors() const { return _Anchors; }
		inline const Vector2& GetAlignment() const { return _Alignment; }
		inline int32 GetZOrder() const { return _ZOrder; }
		inline bool IsAutoSize() const { return _bAutoSize; }
	};

private:
	struct NChildZOrder
	{
		size_t ChildIndex = 0;
		int32 ZOrder = 0;
	};

private:
	std::vector<NSlot> Slots;

public:
	SCanvasPanel();
	virtual ~SCanvasPanel() noexcept override;

	NSlot& AddSlot();
	bool RemoveSlot(size_t Index);
	size_t FindSlot(const SWidget& Content) const;
	void ClearSlots();
	size_t NumSlots() const { return Slots.size(); }

	virtual size_t NumChildrens() const override { return Slots.size(); }
	virtual SWidget* GetChildrenAt(size_t InIndex) const override
	{
		if (Slots.size() < InIndex)
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

	void ArrangeLayeredChildrens(NArrangedChildrens& InoutArrangedChildrens, const NGeometry& AllottedGeometry, std::vector<bool>& ChildLayers) const;

protected:
	BEGIN_SLATE_ATTRIBUTE(SCanvasPanel)
		DECLARE_SLATE_SLOT_SUPPORTS(NSlot)
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();
};