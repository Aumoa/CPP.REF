// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Margin.h"

class SWidget;

struct AlignmentArrangeResult
{
    float Offset = 0;
    float Size = 0;

    constexpr AlignmentArrangeResult(float inOffset, float inSize)
        : Offset(inOffset)
        , Size(inSize)
    {
    }

	template<class TSlot> requires requires { { std::declval<TSlot>().GetContent() } -> std::same_as<const SWidget*>; }
    static AlignmentArrangeResult AlignChild(EOrientation orientation, EFlowDirection inLayoutFlow, float allottedSize, TSlot& childToArrange, Margin slotPadding, float contentScale = 1.0f, bool bClampToParent = true)
    {
	    Margin margin = slotPadding;
	    float totalMargin = margin.GetTotalSpaceAlong(orientation);
	    float marginPre = (orientation == EOrientation::Horizontal) ? margin.Left : margin.Top;
	    float marginPost = (orientation == EOrientation::Horizontal) ? margin.Right : margin.Bottom;

	    int32 alignment = GetChildAlignmentAsInt(orientation, inLayoutFlow, childToArrange);
		if (alignment == (int32)EHorizontalAlignment::Fill)
		{
			return AlignmentArrangeResult(marginPre, (allottedSize - totalMargin) * contentScale);
		}

	    float childDesiredSize = (orientation == EOrientation::Horizontal)
		    ? (childToArrange.GetContent()->GetDesiredSize().X * contentScale)
		    : (childToArrange.GetContent()->GetDesiredSize().Y * contentScale);

	    float childSize = bClampToParent ? MathEx::Min(childDesiredSize, allottedSize - totalMargin) : childDesiredSize;

	    switch (alignment)
	    {
		case (int32)EHorizontalAlignment::Left: // same as Align_Top
		    return AlignmentArrangeResult(marginPre, childSize);
	    case (int32)EHorizontalAlignment::Center:
		    return AlignmentArrangeResult((allottedSize - childSize) / 2.0f + marginPre - marginPost, childSize);
	    case (int32)EHorizontalAlignment::Right: // same as Align_Bottom		
		    return AlignmentArrangeResult(allottedSize - childSize - marginPost, childSize);
	    }

	    // Same as Fill
	    return AlignmentArrangeResult(marginPre, (allottedSize - totalMargin) * contentScale);
    }

private:
	template<class TSlot>
	static int32 GetChildAlignmentAsInt(EOrientation orientation, EFlowDirection inFlowDirection, TSlot& inAlignmentSlot)
	{
		if (orientation == EOrientation::Horizontal)
		{
			switch (inFlowDirection)
			{
			default:
			case EFlowDirection::LeftToRight:
				return (int32)inAlignmentSlot._HAlignment;
			case EFlowDirection::RightToLeft:
				switch (inAlignmentSlot._HAlignment)
				{
				case EHorizontalAlignment::Left:
					return (int32)EHorizontalAlignment::Right;
				case EHorizontalAlignment::Right:
					return (int32)EHorizontalAlignment::Left;
				default:
					return (int32)inAlignmentSlot._HAlignment;
				}
			}
		}
		else
		{
			// InFlowDirection has no effect in vertical orientations.
			return (int32)inAlignmentSlot._VAlignment;
		}
	}
};