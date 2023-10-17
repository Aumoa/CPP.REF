// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SPanel.h"

class NRHIViewport;

class SLATECORE_API SViewport : public SPanel
{
	GENERATED_SLATE_BODY(SViewport)

public:
	struct SLATECORE_API NSlot : public NSlotBase<NSlot>
	{
		DECLARE_SLATE_ATTRIBUTE(int32, ZOrder, 0);
	};

	struct NChildZOrder
	{
		size_t ChildIndex = 0;
		int32 ZOrder = 0;
	};

private:
	std::vector<NSlot> Slots;
	std::shared_ptr<NRHIViewport> Viewport;

public:
	SViewport();
	virtual ~SViewport() noexcept override;

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
	
public:
	BEGIN_SLATE_ATTRIBUTE()
		DECLARE_SLATE_SLOT_SUPPORTS(NSlot)
		DECLARE_SLATE_ATTRIBUTE(bool, bAllocateViewport, false);
	END_SLATE_ATTRIBUTE();

	DECLARE_SLATE_CONSTRUCTOR();

protected:
	virtual Vector2 ComputeDesiredSize() const override;
	virtual int32 OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const override;
	virtual void OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const override;
};