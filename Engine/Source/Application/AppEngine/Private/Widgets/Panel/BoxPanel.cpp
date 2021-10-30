// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Widgets/Panel/BoxPanel.h"
#include "Layout/AlignmentArrangeResult.h"
#include "Layout/ArrangedChildrens.h"
#include <ranges>

SBoxPanel::SBoxPanel(EOrientation InOrientation) : Super()
	, Orientation(InOrientation)
{
}

Vector2 SBoxPanel::GetDesiredSize()
{
	return ComputeDesiredSizeForBox(Orientation, Slots);
}

auto SBoxPanel::AddSlot() -> Slot&
{
	return Slots.emplace_back();
}

bool SBoxPanel::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	return true;
}

size_t SBoxPanel::FindSlot(const SWidget* Content)
{
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		if (Slots[i].GetContent() == Content)
		{
			return i;
		}
	}

	return -1;
}

void SBoxPanel::ClearSlots()
{
	Slots.resize(0);
}

size_t SBoxPanel::NumSlots()
{
	return Slots.size();
}

void SBoxPanel::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ArrangeChildrenAlong(Orientation, GetFlowDirection(), AllottedGeometry, ArrangedChildrens);
}

void SBoxPanel::ArrangeChildrenAlong(EOrientation InOrientation, EFlowDirection InLayoutFlow, const Geometry& AllottedGeometry, ArrangedChildrens& ArrangedChildrens)
{
	// Allotted space will be given to fixed-size children first.
	// Remaining space will be proportionately divided between stretch children (SizeRule_Stretch)
	// based on their stretch coefficient

	if (Slots.size() > 0)
	{
		float StretchCoefficientTotal = 0;
		float FixedTotal = 0;

		bool bAnyChildVisible = false;
        // Compute the sum of stretch coefficients (SizeRule_Stretch) and space required by fixed-size widgets
        // (SizeRule_Auto).
        for (const Slot& CurChild : Slots)
        {
            if (CurChild.GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
            {
                bAnyChildVisible = true;
                // All widgets contribute their margin to the fixed space requirement
                FixedTotal += CurChild._SlotPadding.GetTotalSpaceAlong(InOrientation);

                if (CurChild._SizeParam.SizeRule == ESizeRule::Stretch)
                {
                    // for stretch children we save sum up the stretch coefficients
                    StretchCoefficientTotal += CurChild._SizeParam.Value;
                }
                else
                {
                    Vector2 childDesiredSize = CurChild.GetContent()->GetDesiredSize();

                    // Auto-sized children contribute their desired size to the fixed space requirement
                    float ChildSize = (InOrientation == EOrientation::Vertical)
                        ? childDesiredSize.Y
                        : childDesiredSize.X;

                    // Clamp to the max size if it was specified
                    float MaxSize = CurChild._MaxSize;
                    FixedTotal += MaxSize > 0 ? MathEx::Min(MaxSize, ChildSize) : ChildSize;
                }
            }
        }

		if (!bAnyChildVisible)
		{
			return;
		}

        // The space available for SizeRule_Stretch widgets is any space that wasn't taken up by fixed-sized widgets.
        float NonFixedSpace = MathEx::Max(0.0f, (InOrientation == EOrientation::Vertical)
            ? AllottedGeometry.GetSize().Y - FixedTotal
            : AllottedGeometry.GetSize().X - FixedTotal);

        float PositionSoFar = 0;
		EFlowDirection LayoutFlow = InOrientation == EOrientation::Vertical ? EFlowDirection::LeftToRight : InLayoutFlow;
		
		// Now that we have the total fixed-space requirement and the total stretch coefficients we can
		// arrange widgets top-to-bottom or left-to-right (depending on the orientation).
		auto SlotIterationBody = [&](const Slot& CurChild)
		{
            ESlateVisibility ChildVisibility = CurChild.GetContent()->GetVisibility();

			// Figure out the area allocated to the child in the direction of BoxPanel
			// The area allocated to the slot is ChildSize + the associated margin.
			float ChildSize = 0;
			if (ChildVisibility != ESlateVisibility::Collapsed)
			{
				// The size of the widget depends on its size type
				if (CurChild._SizeParam.SizeRule == ESizeRule::Stretch)
				{
					if (StretchCoefficientTotal > 0)
					{
						// Stretch widgets get a fraction of the space remaining after all the fixed-space requirements are met
						ChildSize = NonFixedSpace * CurChild._SizeParam.Value / StretchCoefficientTotal;
					}
				}
				else
				{
					Vector2 ChildDesiredSize = CurChild.GetContent()->GetDesiredSize();

					// Auto-sized widgets get their desired-size value
					ChildSize = (InOrientation == EOrientation::Vertical)
						? ChildDesiredSize.Y
						: ChildDesiredSize.X;
				}

				// Clamp to the max size if it was specified
				float MaxSize = CurChild._MaxSize;
				if (MaxSize > 0)
				{
					ChildSize = MathEx::Min(MaxSize, ChildSize);
				}
			}

            Margin SlotPadding = LayoutPaddingWithFlow(CurChild._SlotPadding, LayoutFlow);

			Vector2 SlotSize = (InOrientation == EOrientation::Vertical)
				? Vector2(AllottedGeometry.GetSize().X, ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical))
				: Vector2(ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal), AllottedGeometry.GetSize().Y);

			// Figure out the size and local position of the child within the slot			
			auto xAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Horizontal, LayoutFlow, SlotSize.X, CurChild, SlotPadding);
			auto yAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Vertical, LayoutFlow, SlotSize.Y, CurChild, SlotPadding);

			Vector2 LocalPosition = (InOrientation == EOrientation::Vertical)
				? Vector2(xAlignmentResult.Offset, PositionSoFar + yAlignmentResult.Offset)
				: Vector2(PositionSoFar + xAlignmentResult.Offset, yAlignmentResult.Offset);

			Vector2 LocalSize(xAlignmentResult.Size, yAlignmentResult.Size);

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.GetContent(),
				// Child's local position (i.e. position within parent)
				LocalPosition,
				// Child's size
				LocalSize
			));

			if (ChildVisibility != ESlateVisibility::Collapsed)
			{
				// Offset the next child by the size of the current child and any post-child (bottom/right) margin
				PositionSoFar += (InOrientation == EOrientation::Vertical) ? SlotSize.Y : SlotSize.X;
			}
		};

		switch (LayoutFlow)
		{
			case EFlowDirection::LeftToRight:
			{
				const auto& view = Slots;
				std::for_each(view.begin(), view.end(), SlotIterationBody);
				break;
			}
			case EFlowDirection::RightToLeft:
			{
				auto view = std::views::reverse(Slots);
				std::for_each(view.begin(), view.end(), SlotIterationBody);
				break;
			}
		}
	}
}

Vector2 SBoxPanel::ComputeDesiredSizeForBox(EOrientation InOrientation, const std::vector<Slot>& Slots)
{
    // The desired size of this panel is the total size desired by its children plus any margins specified in this panel.
    // The layout along the panel's axis is describe dy the SizeParam, while the perpendicular layout is described by the
    // alignment property.
	Vector2 MyDesiredSize;
    for (auto& curChild : Slots)
    {
        if (curChild.GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
        {
            Vector2 CurChildDesiredSize = curChild.GetContent()->GetDesiredSize();

            if (InOrientation == EOrientation::Vertical)
            {
                // For a vertical panel, we want to find the maximum desired width (including margin).
                // That will be the desired width of the whole panel.
                MyDesiredSize.X = MathEx::Max(MyDesiredSize.X, CurChildDesiredSize.X + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal));

                // Clamp to the max size if it was specified
                float FinalChildDesiredSize = CurChildDesiredSize.Y;
                float MaxSize = curChild._MaxSize;
                if (MaxSize > 0)
                {
                    FinalChildDesiredSize = MathEx::Min(MaxSize, FinalChildDesiredSize);
                }

                MyDesiredSize.Y += FinalChildDesiredSize + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
            }
            else
            {
                // A horizontal panel is just a sideways vertical panel: the axes are swapped.

                MyDesiredSize.Y = MathEx::Max(MyDesiredSize.Y, CurChildDesiredSize.Y + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical));

                // Clamp to the max size if it was specified
                float FinalChildDesiredSize = CurChildDesiredSize.X;
                float MaxSize = curChild._MaxSize;
                if (MaxSize > 0)
                {
                    FinalChildDesiredSize = MathEx::Min(MaxSize, FinalChildDesiredSize);
                }

                MyDesiredSize.X += FinalChildDesiredSize + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
            }
        }
    }

    return MyDesiredSize;
}

DEFINE_SLATE_CONSTRUCTOR(SBoxPanel, Attr)
{
	Super::Construct(Attr);
	Slots = std::move(Attr.Slots);
}