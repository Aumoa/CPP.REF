// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "SlateCore/Layout/Margin.h"

class SWidget;

struct AlignmentArrangeResult
{
    float Offset = 0;
    float Size = 0;

    constexpr AlignmentArrangeResult(float InOffset, float InSize)
        : Offset(InOffset)
        , Size(InSize)
    {
    }

	template<class TSlot> requires requires { { std::declval<TSlot>().GetContent() } -> std::same_as<SWidget*>; }
    static AlignmentArrangeResult AlignChild(EOrientation InOrientation, EFlowDirection InLayoutFlow, float AllottedSize, TSlot& ChildToArrange, Margin SlotPadding, float ContentScale = 1.0f, bool bClampToParent = true)
    {
	    Margin Margin = SlotPadding;
	    float TotalMargin = Margin.GetTotalSpaceAlong(InOrientation);
	    float MarginPre = (InOrientation == EOrientation::Horizontal) ? Margin.Left : Margin.Top;
	    float MarginPost = (InOrientation == EOrientation::Horizontal) ? Margin.Right : Margin.Bottom;

	    int32 Alignment = GetChildAlignmentAsInt(InOrientation, InLayoutFlow, ChildToArrange);
		if (Alignment == (int32)EHorizontalAlignment::Fill)
		{
			return AlignmentArrangeResult(MarginPre, (AllottedSize - TotalMargin) * ContentScale);
		}

	    float ChildDesiredSize = (InOrientation == EOrientation::Horizontal)
		    ? (ChildToArrange.GetContent()->GetDesiredSize().X * ContentScale)
		    : (ChildToArrange.GetContent()->GetDesiredSize().Y * ContentScale);

	    float ChildSize = bClampToParent ? MathEx::Min(ChildDesiredSize, AllottedSize - TotalMargin) : ChildDesiredSize;

	    switch (Alignment)
	    {
		case (int32)EHorizontalAlignment::Left: // same as Align_Top
		    return AlignmentArrangeResult(MarginPre, ChildSize);
	    case (int32)EHorizontalAlignment::Center:
		    return AlignmentArrangeResult((AllottedSize - ChildSize) / 2.0f + MarginPre - MarginPost, ChildSize);
	    case (int32)EHorizontalAlignment::Right: // same as Align_Bottom		
		    return AlignmentArrangeResult(AllottedSize - ChildSize - MarginPost, ChildSize);
	    }

	    // Same as Fill
	    return AlignmentArrangeResult(MarginPre, (AllottedSize - TotalMargin) * ContentScale);
    }

private:
	template<class TSlot>
	static int32 GetChildAlignmentAsInt(EOrientation InOrientation, EFlowDirection InFlowDirection, TSlot& InAlignmentSlot)
	{
		if (InOrientation == EOrientation::Horizontal)
		{
			switch (InFlowDirection)
			{
			default:
			case EFlowDirection::LeftToRight:
				return (int32)InAlignmentSlot._HAlignment;
			case EFlowDirection::RightToLeft:
				switch (InAlignmentSlot._HAlignment)
				{
				case EHorizontalAlignment::Left:
					return (int32)EHorizontalAlignment::Right;
				case EHorizontalAlignment::Right:
					return (int32)EHorizontalAlignment::Left;
				default:
					return (int32)InAlignmentSlot._HAlignment;
				}
			}
		}
		else
		{
			// InFlowDirection has no effect in vertical orientations.
			return (int32)InAlignmentSlot._VAlignment;
		}
	}
};