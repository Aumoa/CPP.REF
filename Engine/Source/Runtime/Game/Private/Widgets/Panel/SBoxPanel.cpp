// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Widgets/Panel/SBoxPanel.h"
#include <ranges>
#include "SBoxPanel.gen.cpp"

SBoxPanel::SBoxPanel(EOrientation Orientation) : Super()
	, Orientation(Orientation)
{
}

void SBoxPanel::SetOrientation(EOrientation Orientation)
{
	if (this->Orientation != Orientation)
	{
		this->Orientation = Orientation;
		InvalidateLayoutAndVolatility();
	}
}

//auto SBoxPanel::AddSlot() -> SSlot&
//{
//	InvalidateLayoutAndVolatility();
//	return *Slots.emplace_back(gcnew SSlot());
//}

//bool SBoxPanel::RemoveSlot(size_t Index)
//{
//	if (Slots.size() <= Index)
//	{
//		return false;
//	}
//
//	Slots.erase(Slots.begin() + Index);
//	InvalidateLayoutAndVolatility();
//	return true;
//}

size_t SBoxPanel::FindSlot(const SWidget* Content)
{
	//for (size_t i = 0; i < Slots.size(); ++i)
	//{
	//	if (Slots[i]->GetContent() == Content)
	//	{
	//		return i;
	//	}
	//}

	return -1;
}

void SBoxPanel::ClearSlots()
{
	//if (Slots.size())
	//{
	//	Slots.resize(0);
	//	InvalidateLayoutAndVolatility();
	//}
}

//size_t SBoxPanel::NumSlots()
//{
//	//return Slots.size();
//}

EOrientation SBoxPanel::GetOrientation()
{
	return Orientation;
}

//size_t SBoxPanel::NumChildrens()
//{
//	return Slots.size();
//}
//
//SWidget* SBoxPanel::GetChildrenAt(size_t IndexOf)
//{
//	return Slots[IndexOf]->GetContent();
//}

//Vector2 SBoxPanel::ComputeDesiredSize()
//{
//	return ComputeDesiredSizeForBox(Orientation, Slots);
//}

void SBoxPanel::OnArrangeChildren(ArrangedChildrens& ArrangedChildrens, const Geometry& AllottedGeometry)
{
	ArrangeChildrenAlong(Orientation, GetFlowDirection(), AllottedGeometry, ArrangedChildrens);
}

void SBoxPanel::ArrangeChildrenAlong(EOrientation InOrientation, EFlowDirection InLayoutFlow, const Geometry& AllottedGeometry, ArrangedChildrens& ArrangedChildrens)
{
	// Allotted space will be given to fixed-size children first.
	// Remaining space will be proportionately divided between stretch children (SizeRule_Stretch)
	// based on their stretch coefficient

	//if (Slots.size() > 0)
	//{
	//	float StretchCoefficientTotal = 0;
	//	float FixedTotal = 0;

	//	bool bAnyChildVisible = false;
 //       // Compute the sum of stretch coefficients (SizeRule_Stretch) and space required by fixed-size widgets
 //       // (SizeRule_Auto).
 //       for (SSlot*& CurChild : Slots)
 //       {
 //           if (CurChild->GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
 //           {
 //               bAnyChildVisible = true;
 //               // All widgets contribute their margin to the fixed space requirement
 //               FixedTotal += CurChild->_SlotPadding.GetTotalSpaceAlong(InOrientation);

 //               if (CurChild->_SizeParam.SizeRule == ESizeRule::Stretch)
 //               {
 //                   // for stretch children we save sum up the stretch coefficients
 //                   StretchCoefficientTotal += CurChild->_SizeParam.Value;
 //               }
 //               else
 //               {
 //                   Vector2 ChildDesiredSize = CurChild->GetContent()->GetDesiredSize();

 //                   // Auto-sized children contribute their desired size to the fixed space requirement
 //                   float ChildSize = (InOrientation == EOrientation::Vertical)
 //                       ? ChildDesiredSize.Y
 //                       : ChildDesiredSize.X;

 //                   // Clamp to the max size if it was specified
 //                   float MaxSize = CurChild->_MaxSize;
 //                   FixedTotal += MaxSize > 0 ? MathEx::Min(MaxSize, ChildSize) : ChildSize;
 //               }
 //           }
 //       }

	//	if (!bAnyChildVisible)
	//	{
	//		return;
	//	}

 //       // The space available for SizeRule_Stretch widgets is any space that wasn't taken up by fixed-sized widgets.
 //       float NonFixedSpace = MathEx::Max(0.0f, (InOrientation == EOrientation::Vertical)
 //           ? AllottedGeometry.GetSize().Y - FixedTotal
 //           : AllottedGeometry.GetSize().X - FixedTotal);

 //       float PositionSoFar = 0;
	//	EFlowDirection LayoutFlow = InOrientation == EOrientation::Vertical ? EFlowDirection::LeftToRight : InLayoutFlow;
	//	
	//	// Now that we have the total fixed-space requirement and the total stretch coefficients we can
	//	// arrange widgets top-to-bottom or left-to-right (depending on the InOrientation).
	//	auto SlotIterationBody = [&](SSlot*& CurChild)
	//	{
 //           ESlateVisibility ChildVisibility = CurChild->GetContent()->GetVisibility();

	//		// Figure out the area allocated to the child in the direction of BoxPanel
	//		// The area allocated to the slot is ChildSize + the associated margin.
	//		float ChildSize = 0;
	//		if (ChildVisibility != ESlateVisibility::Collapsed)
	//		{
	//			// The size of the widget depends on its size type
	//			if (CurChild->_SizeParam.SizeRule == ESizeRule::Stretch)
	//			{
	//				if (StretchCoefficientTotal > 0)
	//				{
	//					// Stretch widgets get a fraction of the space remaining after all the fixed-space requirements are met
	//					ChildSize = NonFixedSpace * CurChild->_SizeParam.Value / StretchCoefficientTotal;
	//				}
	//			}
	//			else
	//			{
	//				Vector2 ChildDesiredSize = CurChild->GetContent()->GetDesiredSize();

	//				// Auto-sized widgets get their desired-size value
	//				ChildSize = (InOrientation == EOrientation::Vertical)
	//					? ChildDesiredSize.Y
	//					: ChildDesiredSize.X;
	//			}

	//			// Clamp to the max size if it was specified
	//			float MaxSize = CurChild->_MaxSize;
	//			if (MaxSize > 0)
	//			{
	//				ChildSize = MathEx::Min(MaxSize, ChildSize);
	//			}
	//		}

 //           Margin SlotPadding = LayoutPaddingWithFlow(CurChild->_SlotPadding, LayoutFlow);

	//		Vector2 SlotSize = (InOrientation == EOrientation::Vertical)
	//			? Vector2(AllottedGeometry.GetSize().X, ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical))
	//			: Vector2(ChildSize + SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal), AllottedGeometry.GetSize().Y);

	//		// Figure out the size and local position of the child within the slot			
	//		auto xAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Horizontal, LayoutFlow, SlotSize.X, *CurChild, SlotPadding);
	//		auto yAlignmentResult = AlignmentArrangeResult::AlignChild(EOrientation::Vertical, LayoutFlow, SlotSize.Y, *CurChild, SlotPadding);

	//		Vector2 LocalPosition = (InOrientation == EOrientation::Vertical)
	//			? Vector2(xAlignmentResult.Offset, PositionSoFar + yAlignmentResult.Offset)
	//			: Vector2(PositionSoFar + xAlignmentResult.Offset, yAlignmentResult.Offset);

	//		Vector2 LocalSize(xAlignmentResult.Size, yAlignmentResult.Size);

	//		// Add the information about this child to the output list (ArrangedChildren)
	//		ArrangedChildrens.AddWidget(ChildVisibility, AllottedGeometry.MakeChild(
	//			// The child widget being arranged
	//			CurChild->GetContent(),
	//			// Child's local position (i.e. position within parent)
	//			LocalPosition,
	//			// Child's size
	//			LocalSize
	//		));

	//		if (ChildVisibility != ESlateVisibility::Collapsed)
	//		{
	//			// Offset the next child by the size of the current child and any post-child (bottom/right) margin
	//			PositionSoFar += (InOrientation == EOrientation::Vertical) ? SlotSize.Y : SlotSize.X;
	//		}
	//	};

	//	switch (LayoutFlow)
	//	{
	//		case EFlowDirection::LeftToRight:
	//		{
	//			auto& View = Slots;
	//			std::for_each(View.begin(), View.end(), SlotIterationBody);
	//			break;
	//		}
	//		case EFlowDirection::RightToLeft:
	//		{
	//			auto View = std::views::reverse(Slots);
	//			std::for_each(View.begin(), View.end(), SlotIterationBody);
	//			break;
	//		}
	//		default:
	//		{
	//			checkf(false, TEXT("The parameter \"InLayoutFlow\"({}) does not valid."), (int32)InLayoutFlow);
	//			break;
	//		}
	//	}
	//}
}

//Vector2 SBoxPanel::ComputeDesiredSizeForBox(EOrientation InOrientation, const std::vector<SSlot*>& slots)
//{
//    // The desired size of this panel is the total size desired by its children plus any margins specified in this panel.
//    // The layout along the panel's axis is describe dy the SizeParam, while the perpendicular layout is described by the
//    // alignment property.
//	Vector2 myDesiredSize;
//    for (auto& CurChild : slots)
//    {
//        if (CurChild->GetContent()->GetVisibility() != ESlateVisibility::Collapsed)
//        {
//            Vector2 CurChildDesiredSize = CurChild->GetContent()->GetDesiredSize();
//
//            if (InOrientation == EOrientation::Vertical)
//            {
//                // For a vertical panel, we want to find the maximum desired width (including margin).
//                // That will be the desired width of the whole panel.
//                myDesiredSize.X = MathEx::Max(myDesiredSize.X, CurChildDesiredSize.X + CurChild->_SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal));
//
//                // Clamp to the max size if it was specified
//                float finalChildDesiredSize = CurChildDesiredSize.Y;
//                float MaxSize = CurChild->_MaxSize;
//                if (MaxSize > 0)
//                {
//                    finalChildDesiredSize = MathEx::Min(MaxSize, finalChildDesiredSize);
//                }
//
//                myDesiredSize.Y += finalChildDesiredSize + CurChild->_SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical);
//            }
//            else
//            {
//                // A horizontal panel is just a sideways vertical panel: the axes are swapped.
//
//                myDesiredSize.Y = MathEx::Max(myDesiredSize.Y, CurChildDesiredSize.Y + CurChild->_SlotPadding.GetTotalSpaceAlong(EOrientation::Vertical));
//
//                // Clamp to the max size if it was specified
//                float finalChildDesiredSize = CurChildDesiredSize.X;
//                float MaxSize = CurChild->_MaxSize;
//                if (MaxSize > 0)
//                {
//                    finalChildDesiredSize = MathEx::Min(MaxSize, finalChildDesiredSize);
//                }
//
//                myDesiredSize.X += finalChildDesiredSize + CurChild->_SlotPadding.GetTotalSpaceAlong(EOrientation::Horizontal);
//            }
//        }
//    }
//
//    return myDesiredSize;
//}

DEFINE_SLATE_CONSTRUCTOR(SBoxPanel, Attr)
{
	INVOKE_SLATE_CONSTRUCTOR_SUPER(Attr);

	//Slots.reserve(Attr.Slots.size());
	//for (auto& Slot : Attr.Slots)
	//{
	//	Slots.emplace_back(gcnew SSlot(std::move(Slot)));
	//}
}