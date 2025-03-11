// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Margin.h"
#include "Layout/FlowDirection.h"
#include "Layout/Orientation.h"
#include "Layout/HorizontalAlignment.h"
class SWidget;

struct NAlignmentArrangeResult
{
	float Offset = 0;
	float Size = 0;

	constexpr NAlignmentArrangeResult(float InOffset, float InSize)
		: Offset(InOffset)
		, Size(InSize)
	{
	}

	template<class TSlot>
	static NAlignmentArrangeResult AlignChild(EOrientation InOrientation, EFlowDirection InLayoutFlow, float AllottedSize, TSlot& ChildToArrange, NMargin SlotPadding, float ContentScale = 1.0f, bool bClampToParent = true)
	{
		NMargin Margin = SlotPadding;
		float TotalMargin = Margin.GetTotalSpaceAlong(InOrientation);
		float MarginPre = (InOrientation == EOrientation::Horizontal) ? Margin.Left : Margin.Top;
		float MarginPost = (InOrientation == EOrientation::Horizontal) ? Margin.Right : Margin.Bottom;

		int32 Alignment = GetChildAlignmentAsInt(InOrientation, InLayoutFlow, ChildToArrange);
		if (Alignment == (int32)EHorizontalAlignment::Fill)
		{
			return NAlignmentArrangeResult(MarginPre, (AllottedSize - TotalMargin) * ContentScale);
		}

		float ChildDesiredSize = (InOrientation == EOrientation::Horizontal)
			? (ChildToArrange.Content->GetDesiredSize().X * ContentScale)
			: (ChildToArrange.Content->GetDesiredSize().Y * ContentScale);

		float ChildSize = bClampToParent ? Math::Min(ChildDesiredSize, AllottedSize - TotalMargin) : ChildDesiredSize;

		switch (Alignment)
		{
		case (int32)EHorizontalAlignment::Left: // same as Align_Top
			return NAlignmentArrangeResult(MarginPre, ChildSize);
		case (int32)EHorizontalAlignment::Center:
			return NAlignmentArrangeResult((AllottedSize - ChildSize) / 2.0f + MarginPre - MarginPost, ChildSize);
		case (int32)EHorizontalAlignment::Right: // same as Align_Bottom		
			return NAlignmentArrangeResult(AllottedSize - ChildSize - MarginPost, ChildSize);
		}

		// Same as Fill
		return NAlignmentArrangeResult(MarginPre, (AllottedSize - TotalMargin) * ContentScale);
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