// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Widgets/Layout/SBoxPanel.h"
#include "Layout/AlignmentArrangeResult.h"

SBoxPanel::SBoxPanel(EOrientation InOrientation)
	: Orientation(InOrientation)
{
}

void SBoxPanel::SetOrientation(EOrientation InOrientation)
{
	if (Orientation != InOrientation)
	{
		Orientation = InOrientation;
		InvalidateLayoutAndVolatility();
	}
}

SBoxPanel::~SBoxPanel() noexcept
{
}

auto SBoxPanel::AddSlot() -> NSlot&
{
	InvalidateLayoutAndVolatility();
	return Slots.emplace_back();
}

bool SBoxPanel::RemoveSlot(size_t Index)
{
	if (Slots.size() <= Index)
	{
		return false;
	}

	Slots.erase(Slots.begin() + Index);
	InvalidateLayoutAndVolatility();
	return true;
}

size_t SBoxPanel::FindSlot(const SWidget& InContent) const
{
	for (size_t i = 0; i < Slots.size(); ++i)
	{
		if (Slots[i].Content.get() == &InContent)
		{
			return i;
		}
	}

	return IntegralTypes::Npos;
}

void SBoxPanel::ClearSlots()
{
	if (Slots.empty() == false)
	{
		Slots.clear();
		InvalidateLayoutAndVolatility();
	}
}

SWidget* SBoxPanel::GetChildrenAt(size_t IndexOf) const
{
	if (Slots.size() <= IndexOf)
	{
		return nullptr;
	}

	return Slots[IndexOf].Content.get();
}

Vector2 SBoxPanel::ComputeDesiredSize() const
{
	return ComputeDesiredSizeForBox(Orientation, Slots);
}

void SBoxPanel::OnArrangeChildren(NArrangedChildrens& ArrangedChildrens, const NGeometry& AllottedGeometry) const
{
	ArrangeChildrenAlong(Orientation, GetFlowDirection(), AllottedGeometry, ArrangedChildrens);
}

int32 SBoxPanel::OnPaint(const NPaintArgs& Args, const NGeometry& AllottedGeometry, const Rect& CullingRect, NSlateWindowElementList& OutDrawElements, int32 InLayer, bool bParentEnabled) const
{
	NArrangedChildrens ArrangedChildren(ESlateVisibility::Visible);
	ArrangeChildrenAlong(Orientation, GetFlowDirection(), AllottedGeometry, ArrangedChildren);

	const bool bForwardedEnabled = ShouldBeEnabled(bParentEnabled);

	// Because we paint multiple children, we must track the maximum layer id that they produced in case one of our parents
	// wants to an overlay for all of its contents.
	int32 MaxLayerId = InLayer;
	int32 ChildLayerId = InLayer;
	NPaintArgs NewArgs = Args.WithNewParent(*this);

	const std::vector<NArrangedWidget>& ArrangedWidgets = ArrangedChildren.GetWidgets();
	for (size_t ChildIndex = 0; ChildIndex < ArrangedWidgets.size(); ++ChildIndex)
	{
		const NArrangedWidget& CurWidget = ArrangedWidgets[ChildIndex];

		if (!IsChildWidgetCulled(CullingRect, CurWidget))
		{
			const int32 CurWidgetsMaxLayerId = CurWidget.GetWidget()->Paint(NewArgs, CurWidget.GetGeometry(), CullingRect, OutDrawElements, ChildLayerId, bForwardedEnabled);
			MaxLayerId = Math::Max(MaxLayerId, CurWidgetsMaxLayerId);
		}
	}

	return MaxLayerId;
}

void SBoxPanel::ArrangeChildrenAlong(EOrientation InOrientation, EFlowDirection InLayoutFlow, const NGeometry& AllottedGeometry, NArrangedChildrens& ArrangedChildrens) const
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
        for (const NSlot& CurChild : Slots)
        {
            if (CurChild.Content->GetVisibility() != ESlateVisibility::Collapsed)
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
                    Vector2 ChildDesiredSize = CurChild.Content->GetDesiredSize();

                    // Auto-sized children contribute their desired size to the fixed space requirement
                    float ChildSize = (InOrientation == EOrientation::Vertical)
                        ? ChildDesiredSize.Y
                        : ChildDesiredSize.X;

                    // Clamp to the max size if it was specified
                    float MaxSize = CurChild._MaxSize;
                    FixedTotal += MaxSize > 0 ? Math::Min(MaxSize, ChildSize) : ChildSize;
                }
            }
        }

		if (!bAnyChildVisible)
		{
			return;
		}

        // The space available for SizeRule_Stretch widgets is any space that wasn't taken up by fixed-sized widgets.
        float NonFixedSpace = Math::Max(0.0f, (InOrientation == EOrientation::Vertical)
            ? AllottedGeometry.GetSize().Y - FixedTotal
            : AllottedGeometry.GetSize().X - FixedTotal);

        float PositionSoFar = 0;
		EFlowDirection LayoutFlow = InOrientation == EOrientation::Vertical ? EFlowDirection::LeftToRight : InLayoutFlow;
		
		// Now that we have the total fixed-space requirement and the total stretch coefficients we can
		// arrange widgets top-to-bottom or left-to-right (depending on the InOrientation).
		auto SlotIterationBody = [&](const NSlot& CurChild)
		{
            ESlateVisibility::Enum ChildVisibility = CurChild.Content->GetVisibility();

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
					Vector2 ChildDesiredSize = CurChild.Content->GetDesiredSize();

					// Auto-sized widgets get their desired-size value
					ChildSize = (InOrientation == EOrientation::Vertical)
						? ChildDesiredSize.Y
						: ChildDesiredSize.X;
				}

				// Clamp to the max size if it was specified
				float MaxSize = CurChild._MaxSize;
				if (MaxSize > 0)
				{
					ChildSize = Math::Min(MaxSize, ChildSize);
				}
			}

            NMargin SlotPadding = LayoutPaddingWithFlow(CurChild._SlotPadding, LayoutFlow);

			Vector2 SlotSize = (InOrientation == EOrientation::Vertical)
				? Vector2(AllottedGeometry.GetSize().X, ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical))
				: Vector2(ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal), AllottedGeometry.GetSize().Y);

			// Figure out the size and local position of the child within the slot			
			auto xAlignmentResult = NAlignmentArrangeResult::AlignChild(EOrientation::Horizontal, LayoutFlow, SlotSize.X, CurChild, SlotPadding);
			auto yAlignmentResult = NAlignmentArrangeResult::AlignChild(EOrientation::Vertical, LayoutFlow, SlotSize.Y, CurChild, SlotPadding);

			Vector2 LocalPosition = (InOrientation == EOrientation::Vertical)
				? Vector2(xAlignmentResult.Offset, PositionSoFar + yAlignmentResult.Offset)
				: Vector2(PositionSoFar + xAlignmentResult.Offset, yAlignmentResult.Offset);

			Vector2 LocalSize(xAlignmentResult.Size, yAlignmentResult.Size);

			// Add the information about this child to the output list (ArrangedChildren)
			ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
				// The child widget being arranged
				CurChild.Content,
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
				auto& View = Slots;
				std::for_each(View.begin(), View.end(), SlotIterationBody);
				break;
			}
			case EFlowDirection::RightToLeft:
			{
				auto View = std::views::reverse(Slots);
				std::for_each(View.begin(), View.end(), SlotIterationBody);
				break;
			}
			default:
			{
				checkf(false, TEXT("The parameter \"InLayoutFlow\"({}) does not valid."), (int32)InLayoutFlow);
				break;
			}
		}
	}
}

Vector2 SBoxPanel::ComputeDesiredSizeForBox(EOrientation InOrientation, const std::vector<NSlot>& Slots)
{
    // The desired size of this panel is the total size desired by its children plus any margins specified in this panel.
    // The layout along the panel's axis is describe dy the SizeParam, while the perpendicular layout is described by the
    // alignment property.
	Vector2 MyDesiredSize;
    for (auto& CurChild : Slots)
    {
        if (CurChild.Content->GetVisibility() != ESlateVisibility::Collapsed)
        {
            Vector2 CurChildDesiredSize = CurChild.Content->GetDesiredSize();

            if (InOrientation == EOrientation::Vertical)
            {
                // For a vertical panel, we want to find the maximum desired width (including margin).
                // That will be the desired width of the whole panel.
                MyDesiredSize.X = Math::Max(MyDesiredSize.X, CurChildDesiredSize.X + CurChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal));

                // Clamp to the max size if it was specified
                float finalChildDesiredSize = CurChildDesiredSize.Y;
                float MaxSize = CurChild._MaxSize;
                if (MaxSize > 0)
                {
                    finalChildDesiredSize = Math::Min(MaxSize, finalChildDesiredSize);
                }

                MyDesiredSize.Y += finalChildDesiredSize + CurChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
            }
            else
            {
                // A horizontal panel is just a sideways vertical panel: the axes are swapped.

                MyDesiredSize.Y = Math::Max(MyDesiredSize.Y, CurChildDesiredSize.Y + CurChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical));

                // Clamp to the max size if it was specified
                float finalChildDesiredSize = CurChildDesiredSize.X;
                float MaxSize = CurChild._MaxSize;
                if (MaxSize > 0)
                {
                    finalChildDesiredSize = Math::Min(MaxSize, finalChildDesiredSize);
                }

                MyDesiredSize.X += finalChildDesiredSize + CurChild._SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
            }
        }
    }

    return MyDesiredSize;
}

DEFINE_SLATE_CONSTRUCTOR(SBoxPanel, Attr)
{
	Slots = std::move(Attr.Slots);
}