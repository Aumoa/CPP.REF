// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Widgets/Panel/BoxPanel.h"

SBoxPanel::SBoxPanel(const std::wstring& name, EOrientation orientation) : Super(name)
	, _orientation(orientation)
{
}

SBoxPanel::~SBoxPanel()
{
}

Vector2 SBoxPanel::GetDesiredSize() const
{
	return ComputeDesiredSizeForBox(_orientation, _slots);
}

auto SBoxPanel::AddSlot() -> Slot&
{
	return _slots.emplace_back();
}

bool SBoxPanel::RemoveSlot(size_t index)
{
	if (_slots.size() <= index)
	{
		return false;
	}

	_slots.erase(_slots.begin() + index);
	return true;
}

size_t SBoxPanel::FindSlot(const SWidget* content) const
{
	for (size_t i = 0; i < _slots.size(); ++i)
	{
		if (_slots[i].GetContent() == content)
		{
			return i;
		}
	}

	return -1;
}

void SBoxPanel::ClearSlots()
{
	_slots.resize(0);
}

size_t SBoxPanel::NumSlots() const
{
	return _slots.size();
}

void SBoxPanel::OnArrangeChildren(SArrangedChildrens* arrangedChildrens, const Geometry& allottedGeometry) const
{
	ArrangeChildrenAlong(_orientation, GetFlowDirection(), allottedGeometry, arrangedChildrens);
}

void SBoxPanel::ArrangeChildrenAlong(EOrientation orientation, EFlowDirection inLayoutFlow, const Geometry& allottedGeometry, SArrangedChildrens* arrangedChildrens) const
{
	// Allotted space will be given to fixed-size children first.
	// Remaining space will be proportionately divided between stretch children (SizeRule_Stretch)
	// based on their stretch coefficient

	if (_slots.size() > 0)
	{
		float stretchCoefficientTotal = 0;
		float fixedTotal = 0;

		bool bAnyChildVisible = false;
        // Compute the sum of stretch coefficients (SizeRule_Stretch) and space required by fixed-size widgets
        // (SizeRule_Auto).
        for (const Slot& curChild : _slots)
        {
            if (curChild.GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
            {
                bAnyChildVisible = true;
                // All widgets contribute their margin to the fixed space requirement
                fixedTotal += curChild._SlotPadding.GetTotalSpaceAlong(orientation);

                if (curChild._SizeParam.SizeRule == ESizeRule::Stretch)
                {
                    // for stretch children we save sum up the stretch coefficients
                    stretchCoefficientTotal += curChild._SizeParam.Value;
                }
                else
                {
                    Vector2 childDesiredSize = curChild.GetContent()->GetDesiredSize();

                    // Auto-sized children contribute their desired size to the fixed space requirement
                    float childSize = (orientation == EOrientation::Vertical)
                        ? childDesiredSize.Y
                        : childDesiredSize.X;

                    // Clamp to the max size if it was specified
                    float maxSize = curChild._MaxSize;
                    fixedTotal += maxSize > 0 ? MathEx::Min(maxSize, childSize) : childSize;
                }
            }
        }

		if (!bAnyChildVisible)
		{
			return;
		}

        // The space available for SizeRule_Stretch widgets is any space that wasn't taken up by fixed-sized widgets.
        float nonFixedSpace = MathEx::Max(0.0f, (orientation == EOrientation::Vertical)
            ? allottedGeometry.GetSize().Y - fixedTotal
            : allottedGeometry.GetSize().X - fixedTotal);

        float positionSoFar = 0;
		EFlowDirection layoutFlow = orientation == EOrientation::Vertical ? EFlowDirection::LeftToRight : inLayoutFlow;
		
		// Now that we have the total fixed-space requirement and the total stretch coefficients we can
		// arrange widgets top-to-bottom or left-to-right (depending on the orientation).
		auto slotIterationBody = [&](const Slot& curChild)
		{
            ESlateVisibility childVisibility = curChild.GetContent()->GetVisibility();

			// Figure out the area allocated to the child in the direction of BoxPanel
			// The area allocated to the slot is ChildSize + the associated margin.
			float childSize = 0;
			if (childVisibility != ESlateVisibility::Collapsed)
			{
				// The size of the widget depends on its size type
				if (curChild._SizeParam.SizeRule == ESizeRule::Stretch)
				{
					if (stretchCoefficientTotal > 0)
					{
						// Stretch widgets get a fraction of the space remaining after all the fixed-space requirements are met
						childSize = nonFixedSpace * curChild._SizeParam.Value / stretchCoefficientTotal;
					}
				}
				else
				{
					Vector2 childDesiredSize = curChild.GetContent()->GetDesiredSize();

					// Auto-sized widgets get their desired-size value
					childSize = (orientation == EOrientation::Vertical)
						? childDesiredSize.Y
						: childDesiredSize.X;
				}

				// Clamp to the max size if it was specified
				float maxSize = curChild._MaxSize;
				if (maxSize > 0)
				{
					childSize = MathEx::Min(maxSize, childSize);
				}
			}

            Margin slotPadding = LayoutPaddingWithFlow(curChild._SlotPadding, layoutFlow);

			Vector2 slotSize = (orientation == EOrientation::Vertical)
				? Vector2(allottedGeometry.GetSize().X, childSize + slotPadding.GetTotalSpaceAlong(EOrientation::Vertical))
				: Vector2(childSize + slotPadding.GetTotalSpaceAlong(EOrientation::Horizontal), allottedGeometry.GetSize().Y);

			// Figure out the size and local position of the child within the slot			
			auto xAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Horizontal, layoutFlow, slotSize.X, curChild, slotPadding);
			auto yAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Vertical, layoutFlow, slotSize.Y, curChild, slotPadding);

			Vector2 localPosition = (orientation == EOrientation::Vertical)
				? Vector2(xAlignmentResult.Offset, positionSoFar + yAlignmentResult.Offset)
				: Vector2(positionSoFar + xAlignmentResult.Offset, yAlignmentResult.Offset);

			Vector2 localSize(xAlignmentResult.Size, yAlignmentResult.Size);

			// Add the information about this child to the output list (ArrangedChildren)
			arrangedChildrens->AddWidget(childVisibility, allottedGeometry.MakeChild(
				// The child widget being arranged
				curChild.GetContent(),
				// Child's local position (i.e. position within parent)
				localPosition,
				// Child's size
				localSize
			));

			if (childVisibility != ESlateVisibility::Collapsed)
			{
				// Offset the next child by the size of the current child and any post-child (bottom/right) margin
				positionSoFar += (orientation == EOrientation::Vertical) ? slotSize.Y : slotSize.X;
			}
		};

		switch (layoutFlow)
		{
			case EFlowDirection::LeftToRight:
			{
				const auto& view = _slots;
				std::for_each(view.begin(), view.end(), slotIterationBody);
				break;
			}
			case EFlowDirection::RightToLeft:
			{
				auto view = std::views::reverse(_slots);
				std::for_each(view.begin(), view.end(), slotIterationBody);
				break;
			}
			default:
			{
				SE_LOG(LogSlate, Error, L"The parameter \"inLayoutFlow\"({}) does not valid.", (int32)inLayoutFlow);
				break;
			}
		}
	}
}

Vector2 SBoxPanel::ComputeDesiredSizeForBox(EOrientation orientation, const std::vector<Slot>& slots)
{
    // The desired size of this panel is the total size desired by its children plus any margins specified in this panel.
    // The layout along the panel's axis is describe dy the SizeParam, while the perpendicular layout is described by the
    // alignment property.
	Vector2 myDesiredSize;
    for (auto& curChild : slots)
    {
        if (curChild.GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
        {
            Vector2 curChildDesiredSize = curChild.GetContent()->GetDesiredSize();

            if (orientation == EOrientation::Vertical)
            {
                // For a vertical panel, we want to find the maximum desired width (including margin).
                // That will be the desired width of the whole panel.
                myDesiredSize.X = MathEx::Max(myDesiredSize.X, curChildDesiredSize.X + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal));

                // Clamp to the max size if it was specified
                float finalChildDesiredSize = curChildDesiredSize.Y;
                float maxSize = curChild._MaxSize;
                if (maxSize > 0)
                {
                    finalChildDesiredSize = MathEx::Min(maxSize, finalChildDesiredSize);
                }

                myDesiredSize.Y += finalChildDesiredSize + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
            }
            else
            {
                // A horizontal panel is just a sideways vertical panel: the axes are swapped.

                myDesiredSize.Y = MathEx::Max(myDesiredSize.Y, curChildDesiredSize.Y + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical));

                // Clamp to the max size if it was specified
                float finalChildDesiredSize = curChildDesiredSize.X;
                float maxSize = curChild._MaxSize;
                if (maxSize > 0)
                {
                    finalChildDesiredSize = MathEx::Min(maxSize, finalChildDesiredSize);
                }

                myDesiredSize.X += finalChildDesiredSize + curChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
            }
        }
    }

    return myDesiredSize;
}

DEFINE_SLATE_CONSTRUCTOR(SBoxPanel, attr)
{
	Super::Construct(attr);
	_slots = std::move(attr.Slots);
}